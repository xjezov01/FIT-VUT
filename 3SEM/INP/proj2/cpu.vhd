-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpstophalter)
-- Copyright (C) 2014 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): FILIP JEZOVICA : xjezov01@fit.vutbr.cz 
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu

-- ------------ INSTRUKCIE -----------------------------------
type instr is (ptr_inc, ptr_dec, bunka_inc, bunka_dec, wh_start, wh_end, put, get, stophalt, nist);
signal INSTRUKCIA : instr;

-- --------------   FSM -------------------------------------------
type MYFSM is ( S_exec, S_WAIT, S_fetch, S_decode, S_ptr_inc, S_ptr_dec, S_bunka_inc, S_bunka_inc2, S_bunka_dec, S_bunka_dec2, S_wh_start, S_wh_end, S_put, S_put2, S_get, S_get2, S_stophalt, S_nist, S_wh_end_CNT, S_wh_end_dalsi, S_wh_end2, S_wh_end1, S_wh_start_CNT, S_wh_start_dalsi, S_wh_start2, S_wh_start1);
signal S_PRESENT : MYFSM; -- actual state
signal S_NEXT : MYFSM; -- next state


-- ------------- CNT -------------------------------------
signal reg_CNT_out : std_logic_vector(7 downto 0);
signal reg_CNT_inc : std_logic;
signal reg_CNT_dec : std_logic;
-- ------------- PC -------------------------------------
signal reg_PC_out : std_logic_vector(12 downto 0);
signal reg_PC_inc : std_logic;
signal reg_PC_dec : std_logic;
-- ------------- PTR -------------------------------------
signal reg_PTR_out : std_logic_vector(12 downto 0);
signal reg_PTR_inc : std_logic;
signal reg_PTR_dec : std_logic;

-- ------------- MULTIPLEXORY -------------------------------------
signal SELMX1 : std_logic;
signal SELMX2 : std_logic_vector(1 downto 0);



begin

-- zde dopiste vlastni VHDL kod

-- --------------------------- REGISTRE -------------------------------------------------

 -- ------------ CNT ------------
    CNT: process(CLK, RESET)
    begin
    	if RESET = '1' then	-- asynchronny reset
    		reg_CNT_out <= X"00";
        elsif CLK'event and CLK = '1' then
            if (reg_CNT_inc = '1') then
            	reg_CNT_out <= reg_CNT_out + X"01";
        	elsif (reg_CNT_dec = '1') then
           		reg_CNT_out <= reg_CNT_out - X"01";
           	end if;
        end if;
    end process CNT;



  -- ------------ PC ------------
    PC: process(CLK, RESET)
    begin
    	if RESET = '1' then	-- asynchronny reset
    		reg_PC_out <= (others => '0');
        elsif CLK'event and CLK = '1' then
            if (reg_PC_inc = '1') then
            	reg_PC_out <= reg_PC_out + "0000000000001";
        	elsif (reg_PC_dec = '1') then
           		reg_PC_out <= reg_PC_out - "0000000000001";
           	end if;
        end if;
    end process PC;



  -- ------------ PTR ------------
    PTR: process(CLK, RESET)
    begin
    	if RESET = '1' then	-- asynchronny reset
    		reg_PTR_out <= "1000000000000";
        elsif CLK'event and CLK = '1' then
            if (reg_PTR_inc = '1') then
            	reg_PTR_out <= reg_PTR_out + "0000000000001";
        	elsif (reg_PTR_dec = '1') then
           		reg_PTR_out <= reg_PTR_out - "0000000000001";
           	end if;
        end if;
    end process PTR;

-- --------------------------- REGISTRE --hore-----------------------------------------------


-- --------------------------- MULTIPLEXORY -------------------------------------------------

  ----------------- MX 1 -----------------------------------------
    process(SELMX1, reg_PC_out, reg_PTR_out)
    begin
    	case SELMX1 is
			when '0' => DATA_ADDR <= reg_PC_out;
			when '1' => DATA_ADDR <= reg_PTR_out;
			when others => --DATA_ADDR <= (others => '0');
		end case;
    	
    end process;

  ----------------- MX 2 -----------------------------------------
  process(SELMX2, IN_DATA, DATA_RDATA )
  begin
    	case SELMX2 is
			when "00" => DATA_WDATA <= IN_DATA;
			when "01" => DATA_WDATA <= DATA_RDATA + X"01"; 
			when "10" => DATA_WDATA <= DATA_RDATA - X"01";
			when others => DATA_WDATA <= X"00";
		end case;

    end process ;

-- --------------------------- MULTIPLEXORY ---hore----------------------------------------------


-- ---------- DEKODER -----------------------------
DEKODER : process(DATA_RDATA)
begin
   case DATA_RDATA is
      when X"3E" => INSTRUKCIA <= ptr_inc; -- >
      when X"3C" => INSTRUKCIA <= ptr_dec; -- <
      when X"2B" => INSTRUKCIA <= bunka_inc; -- +
      when X"2D" => INSTRUKCIA <= bunka_dec; -- -
      when X"5B" => INSTRUKCIA <= wh_start; -- [
      when X"5D" => INSTRUKCIA <= wh_end; -- ]
      when X"2E" => INSTRUKCIA <= put; -- .
      when X"2C" => INSTRUKCIA <= get; -- ,
      when X"00" => INSTRUKCIA <= stophalt; -- NULL
      when others => INSTRUKCIA <= nist;
   end case;
end process;


-- ---------------------------------- FSM ------------------------------------------------

-- prvy stav  PRESENT -------------------------------------------------------------------

-- PRESENT -----------------------
process( CLK, RESET, EN, S_NEXT)
begin
	if RESET = '1' then	-- asynchronny reset
    	S_PRESENT <= S_WAIT;
    elsif CLK'event and CLK = '1' then
    	if EN = '1' then
    		S_PRESENT <= S_NEXT;
    	end if ;
    end if;
	
end process ; -- PRESENT

-- NEXT -----------------------
process( S_PRESENT, S_NEXT, OUT_BUSY, DATA_RDATA, IN_VLD, SELMX1, SELMX2, INSTRUKCIA, reg_CNT_out, reg_PTR_out, reg_PC_out )
begin

	-- VYCHODZI STAV -------------------------------------------
	-- treba VZDY !!! odstavit signal inc,dec na registroch
	reg_CNT_inc <= '0';
	reg_CNT_dec <= '0';
	reg_PTR_inc <= '0';
	reg_PTR_dec <= '0';
	reg_PC_inc <= '0';
	reg_PC_dec <= '0';
	-- --
	SELMX2 <= "11"; -- reset DATA_WDATA
	DATA_RDWR <= '0';
	DATA_EN <= '0';
	IN_REQ	<= '0';
	OUT_WE	<= '0';
	-- ---------------------------------------------------------


	case( S_PRESENT ) is
		when S_WAIT =>
		-- pomocny stav .. musime dockat chvilku dokym sa vsetko nastavi .. vyuzite len pri RESET
			S_NEXT <= S_fetch;

		-- STOP
		when S_stophalt =>
			S_NEXT <= S_stophalt;

		-- "OSTATNI"
		when S_nist =>
			-- PC ← PC + 1
			reg_PC_inc <= '1';
			-- OK dalsia instrukcia
			S_NEXT <= S_fetch;


		when S_fetch =>
			-- nacita nam instrukciu
			DATA_EN <= '1';
			SELMX1 <= '0';

			S_NEXT <= S_decode;


-- -OK----------- DEKODOVANIE INSTRUKCIE start -----------------------------------------------------------
		when S_decode =>
			-- chvilkove spozdenie
			S_NEXT <= S_exec;

		when S_exec =>
			case( INSTRUKCIA ) is
			-- type instr is (ptr_inc, ptr_dec, bunka_inc, bunka_dec, wh_start, wh_end, put, get, stophalt, nist);

				when ptr_inc =>
					S_NEXT <= S_ptr_inc; -- execute

				when ptr_dec =>
					S_NEXT <= S_ptr_dec; -- execute

				when bunka_inc =>
					S_NEXT <= S_bunka_inc; -- execute

				when bunka_dec =>
					S_NEXT <= S_bunka_dec; -- execute

				when wh_start =>
					S_NEXT <= S_wh_start; -- execute

				when wh_end =>
					S_NEXT <= S_wh_end; -- execute

				when put =>
					S_NEXT <= S_put; -- execute

				when get =>
					S_NEXT <= S_get; -- execute

				when stophalt =>
					S_NEXT <= S_stophalt; -- execute

				when others =>
					S_NEXT <= S_nist; -- execute
			
			end case ;
-- ---------- DEKODOVANIE INSTRUKCIE KONIEC -----------------------------------------------------------



-- -OK----------EXECUTE--------------------------------------------------------------------------------

-- -OK POSUV NA BUNKU V PAMETI -- <<   >   >> a <<   <   >> -------------------------------------------
		when S_ptr_inc =>
			-- PTR ← PTR + 1
			reg_PTR_inc <= '1'; -- <<   >   >> posunie sa o bunku v pameti dalej

			-- PC ← PC + 1
			reg_PC_inc <= '1'; -- dalsia instruckia

			-- OK dalsia instrukcia
			S_NEXT <= S_fetch; 

		when S_ptr_dec =>
			-- PTR ← PTR - 1
			reg_PTR_dec <= '1'; -- <<   <   >> posunie sa o bunku v pameti spet

			-- PC ← PC + 1
			reg_PC_inc <= '1'; -- dalsia instruckia

			-- OK dalsia instrukcia
			S_NEXT <= S_fetch;

-- --------POSUV NA BUNKU V PAMETI  KONIEC ------------------------------------------------------------


-- -OK------------------MEM INCREMENT  << + >> --------------------------------
			-- memory ... pristup k pameti
		when S_bunka_inc =>
			-- nastavime si datove cesty
			SELMX1 <= '1' ; -- ram[PTR]

			DATA_EN <= '1' ; -- POVOLIME CINNOST
			DATA_RDWR <= '0' ; -- ideme CITAT
			
			-- PC ← PC + 1
			reg_PC_inc <= '1';

			-- DATA RDATA ← ram[PTR]
			-- teraz to mame v DATA_RDATA
			S_NEXT <= S_bunka_inc2; -- do dalsieho stavu

			-- write > ulozenie
		when S_bunka_inc2 => -- pomocny stav lebo sa neda naraz aj citat aj zapisovat
			SELMX1 <= '1' ; -- ram[PTR]
			
			DATA_EN <= '1'; -- POVOLIME CINNOST
			DATA_RDWR <= '1'; -- ideme ZAPISAT

			-- ram[PTR] ← DATA RDATA + 1
			SELMX2 <= "01" ;

			-- OK dalsia instrukcia
			S_NEXT <= S_fetch; -- ok bunku sme inkrementovali ideme dalej

-- -------------------MEM INCREMENT---hore- << + >> ------------------------



-- -OK------------------MEM DECREMENT---dole- << - >> ------------------------
		when S_bunka_dec =>
			-- nastavime si datove cesty
			SELMX1 <= '1' ; -- ram[PTR]

			DATA_EN <= '1' ; -- POVOLIME CINNOST
			DATA_RDWR <= '0' ; -- ideme CITAT
			
			-- PC ← PC + 1
			reg_PC_inc <= '1';

			-- DATA RDATA ← ram[PTR]
			-- teraz to mame v DATA_RDATA
			S_NEXT <= S_bunka_dec2;

			-- write > ulozenie
		when S_bunka_dec2 => -- pomocny stav lebo sa neda naraz aj citat aj zapisovat
			SELMX1 <= '1' ; -- ram[PTR]
			
			DATA_EN <= '1'; -- POVOLIME CINNOST
			DATA_RDWR <= '1'; -- ideme ZAPISAT

			-- ram[PTR] ← DATA RDATA - 1
			SELMX2 <= "10" ;

			-- OK dalsia instrukcia
			S_NEXT <= S_fetch;

-- -------------------MEM DECREMENT---hore- << - >> ------------------------




-- -OK-----------PIS NA DISPLAY----dole----<< . >>--------------------------------------------------
		when S_put =>
			-- nastavime si datove cesty
			SELMX1 <= '1' ; -- ram[PTR]
			DATA_EN <= '1'; -- POVOLIME CINNOST
			DATA_RDWR <= '0'; -- ideme CITAT

			-- PC ← PC + 1
			reg_PC_inc <= '1';

			S_NEXT <= S_put2; -- do pomocneho stavu

			
		when S_put2 => -- while (OUT BUSY) {}

			if OUT_BUSY = '0' then -- dockali sme sa .. budeme pisat na display
				-- OUT DATA ← ram[PTR]
				OUT_DATA <= DATA_RDATA; -- dame to na out
				OUT_WE <= '1'; -- povolime zapis na displaj

				-- OK dalsia instrukcia
				S_NEXT <= S_fetch;
			else
				S_NEXT <= S_put2; -- ked musime cakat na display tak sa tu tocime
			end if ;
-- ------------PIS NA DISPLAY----hore--------<< . >>----------------------------------------------



-- -OK-----------NACITAJ Z KLAVESICE----dole------<< , >>------------------------------------------------
		when S_get =>
			-- IN REQ ← 1
			IN_REQ <= '1'; -- poziadavka na VSTUP

			-- PC ← PC + 1
			reg_PC_inc <= '1';

			S_NEXT <= S_get2; -- do pomoneho stavu lebo sa to neda urobit na 1 raz vsetko


		when S_get2 =>
			-- IN REQ ← 1
			IN_REQ <= '1'; -- poziadavka na VSTUP

			-- while (!IN VLD) {}
			if IN_VLD = '1' then -- dockali sme sa.. vstup je VALIDNY
				-- nastavime datove cesty
				SELMX1 <= '1' ; -- PTR
				DATA_EN <= '1'; -- POVOLIME CINNOST
				DATA_RDWR <= '1'; -- ideme ZAPISAT

				-- ram[PTR] ← IN DATA
				SELMX2 <= "00" ; -- zapis z IN_DATA teda od uzivatela z klavesnice

				-- OK dalsia instrukcia
				S_NEXT <= S_fetch; -- mozme ist dalej
			else
				S_NEXT <= S_get2; -- este sa tu potocime ... este vstup neni dokonceny (validny)
				
			end if;
-- ------------NACITAJ Z KLAVESICE----hore-------<< , >>-----------------------------------------------



-- ------------WHILE----dole----------------------<< [] >>--------------------------------
-- TU ZACINA   -- WHILE START -- ------------------<< [ >>----------------------------
		when S_wh_start =>
			reg_PC_inc <= '1'; -- PC ← PC + 1
			-- if (ram[PTR] == 0) potrebujem si najprv nacitat ram[PTR] ostatne v pomocnom stave
			-- nastavime si datove cesty
			SELMX1 <= '1' ; -- ram[PTR]

			DATA_EN <= '1' ; -- POVOLIME CINNOST
			DATA_RDWR <= '0' ; -- ideme CITAT
			-- teraz to mame v DATA_RDATA

			-- ostatne sa da az v dalsich taktoch
			S_NEXT <= S_wh_start1;
			
	-- if (ram[PTR] == 0)
		when S_wh_start1 =>
			-- mame uz nacitane ram[PTR] z minuleho stavu
			if DATA_RDATA = X"00" then
				--reg_CNT_out <= X"01"; / -- CNT ← 1
				reg_CNT_inc <= '1'; -- CNT ← 1
				S_NEXT <= S_wh_start2;
			else
				-- OK dalsia instrukcia
				S_NEXT <= S_fetch; -- CYKLUS NEPREBEHNE!
			end if ;
	

		when S_wh_start2 =>
			-- nacitame dalsiu instrukciu
			-- c ← rom[PC]
			SELMX1 <= '0' ; -- PC
			DATA_EN <= '1' ; -- POVOLIME CINNOST
			
			-- nacitame instruckiu v postupnosti dopredu / vykonavame instrukcie vo while
			S_NEXT <= S_wh_start_dalsi;


	-- if (c == ’[’) CNT ← CNT + 1 elsif (c == ’]’) CNT ← CNT - 1
		when S_wh_start_dalsi =>
			-- (c == ’[’)
			if INSTRUKCIA = wh_start then
				reg_CNT_inc <= '1'; -- CNT ← CNT + 1

			-- (c == ’]’)
			elsif INSTRUKCIA = wh_end then
				reg_CNT_dec <= '1'; -- CNT ← CNT - 1
			end if ;

			-- PC ← PC + 1
			reg_PC_inc <= '1'; 

			S_NEXT <= S_wh_start_CNT; -- while (CNT != 0)



		when S_wh_start_CNT =>
			-- while (CNT != 0)
			if reg_CNT_out = X"00" then
				-- OK dalsia instrukcia
				S_NEXT <= S_fetch;
			else
				-- este neni CNT 0
				S_NEXT <= S_wh_start2;
			end if ;

-- TU KONCI   -- WHILE START -- ---------<< [ >>--------------------------------------





-- ------ WHILE END -- ------------------<< ] >>--------------------------------------
		when S_wh_end =>
			-- if (ram[PTR] == 0) potrebujem si najprv nacitat ram[PTR] ostatne v pomocnom stave
			-- nastavime si datove cesty
			SELMX1 <= '1' ; -- ram[PTR]

			DATA_EN <= '1' ; -- POVOLIME CINNOST
			DATA_RDWR <= '0' ; -- ideme CITAT
			
			-- teraz to mame v DATA_RDATA
			S_NEXT <= S_wh_end1;

	-- if (ram[PTR] == 0)
		when S_wh_end1 =>
			if DATA_RDATA = X"00" then
				reg_PC_inc <= '1'; -- PC ← PC + 1
				-- OK dalsia instrukcia
				S_NEXT <= S_fetch;
			else
				-- ESTE pojdeme cely cyklus.. musime sa vratit po instrukcii az na zaciatok kde zacinal WHILE
				--reg_CNT_out <= X"01"; / -- CNT++
				-- CNT ← 1
				reg_CNT_inc <= '1';
				-- PC ← PC - 1
				reg_PC_dec <= '1';

				-- toto sa nam porobi v tomto takte
				-- takze ostatne musi byt az na dalsie takty = pomocny stav
				S_NEXT <= S_wh_end2;
			end if ;

		when S_wh_end2 =>
			-- c ← rom[PC]
			-- instrukcie sa nacitaju v spetnom chode!
			-- nastavime si datove cesty
			SELMX1 <= '0' ; --  c ← rom[PC]

			DATA_EN <= '1' ; -- POVOLIME CINNOST

			-- toto sa mi ulozi ale akcie stym az na dalsi takt (dalsi pomocny stav)
			S_NEXT <= S_wh_end_dalsi;

	-- if (c == ’]’) CNT ← CNT + 1 elsif (c == ’[’) CNT ← CNT - 1
		when S_wh_end_dalsi =>
		    -- (c == ’]’)
			if INSTRUKCIA = wh_end then
				reg_CNT_inc <= '1'; -- CNT ← CNT + 1

			-- (c == ’[’)
			elsif INSTRUKCIA = wh_start then
				-- ok uz sme narazili na zaciatok budeme moct vykonavat zase dopredu
				reg_CNT_dec <= '1'; -- CNT ← CNT - 1
			end if ;

			--if reg_CNT_out = X"00"  sa mozme opytat az o takt dalej = dalsi pomocny stav
			S_NEXT <= S_wh_end_CNT; -- while (CNT != 0)


	-- while (CNT != 0)
		when S_wh_end_CNT =>
			-- if (CNT == 0)
			if reg_CNT_out = X"00" then
			    -- vykonavame cyklus 
			    -- PC ← PC + 1
				reg_PC_inc <= '1';

				-- OK dalsia instrukcia
				S_NEXT <= S_fetch;

			else
				-- PC ← PC - 1
				reg_PC_dec <= '1';
				-- este sa vracame naspet na zaciatok WHILE
				S_NEXT <= S_wh_end2;
				
			end if ;

-- TU KONCI   -- WHILE END -- ---------<< ] >>--------------------------------------

-- -------- KONIEC WHILE -----------<< [] >>----------------------------------------
	
		when others =>
	
	end case ;

	
end process ; -- NEXT

end behavioral;
