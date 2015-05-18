-- fsm.vhd: Finite State Machine
-- Author(s):  Filip Jezovica	xjezov01
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------

-- tu zacina moja praca...

architecture behavioral of fsm is
   type t_state is (TST1, TST2, TST3, TST4, TST5A, TST6A, TST7A, TST8A, TST9A, TST10A, TST5B, TST6B, TST7B, TST8B, TST9B, TST10B, TST_FAJRONT, PRINT_ODEPREN, PRINT_OK_MSG, FALSE_CAKAJ, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TST1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

--  -------------------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
--  -------------------------------------------------------------------
		when TST1 =>
			next_state <= TST1;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(2) = '1') then
				next_state <= TST2;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
		
--  -------------------------------------------------------------------
		when TST2 =>
			next_state <= TST2;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(3) = '1') then
				next_state <= TST3;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
			
--  -------------------------------------------------------------------
		when TST3 =>
			next_state <= TST3;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(4) = '1') then
				next_state <= TST4;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
			
--  -------------------------------------------------------------------
		when TST4 =>
			next_state <= TST4;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(8) = '1') then
				next_state <= TST5A;
			elsif (KEY(5) = '1') then
				next_state <= TST5B;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;

--  ---------------zaciatok VETVY A---------------------------------------------------------------------------zaciatok VETVY A--------------------------
		-- vetva A 
        when TST5A =>
			next_state <= TST5A;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este 
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(3) = '1') then
				next_state <= TST6A;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
		
--  -------------------------------------------------------------------
			-- vetva A 
        when TST6A =>
			next_state <= TST6A;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(6) = '1') then
				next_state <= TST7A;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
		
--  -------------------------------------------------------------------
			-- vetva A 
        when TST7A =>
			next_state <= TST7A;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este 
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(2) = '1') then
				next_state <= TST8A;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
		
--  -------------------------------------------------------------------
			-- vetva A 
        when TST8A =>
			next_state <= TST8A;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este 
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(9) = '1') then
				next_state <= TST9A;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
		
--  -------------------------------------------------------------------
			-- vetva A 
        when TST9A =>
			next_state <= TST9A;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(0) = '1') then
				next_state <= TST10A;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
		
--  -------------------------------------------------------------------
			-- vetva A 
        when TST10A =>
			next_state <= TST10A;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(6) = '1') then
				next_state <= TST_FAJRONT;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
		
--  --------------koniec VETVY A-------------------------------
	
--	---------------zaciatok VETVY B-------------------------------------------------------------------------------B ZACIATOK-----------------
		-- vetva B 
        when TST5B =>
			next_state <= TST5B;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(5) = '1') then
				next_state <= TST6B;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
		
--  -------------------------------------------------------------------
			-- vetva B 
        when TST6B =>
			next_state <= TST6B;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(7) = '1') then
				next_state <= TST7B;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
		
--  -------------------------------------------------------------------
			-- vetva B
        when TST7B =>
			next_state <= TST7B;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(5) = '1') then
				next_state <= TST8B;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
		
--  -------------------------------------------------------------------
			-- vetva B
        when TST8B =>
			next_state <= TST8B;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(2) = '1') then
				next_state <= TST9B;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
		
--  -------------------------------------------------------------------
			-- vetva B
        when TST9B =>
			next_state <= TST9B;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(7) = '1') then
				next_state <= TST10B;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
		
--  -------------------------------------------------------------------
			-- vetva B
        when TST10B =>
			next_state <= TST10B;
--			-- ak dame mriezku tak nemozeme mat pass dobre lebo je malo znakov este
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			elsif (KEY(1) = '1') then
				next_state <= TST_FAJRONT;
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;

--	----------------koniec VETVY B----------------------------
	
--	---------------POTVRDENIE MRIEZKOV---------------------------------------------------------------POTVRDENIE MRIEZKOV--------------
	
			-- SEM SA DOSTANE PLATNY KOD... UZ LEN STACI POTVRDIT MRIEZKOV
        when TST_FAJRONT =>
			next_state <= TST_FAJRONT;
			-- potvrdime mriezkov ak sa dostal az sem tak kod je OK...
			if (KEY(15) = '1') then
				next_state <= PRINT_OK_MSG; 
			elsif (KEY(14 downto 0) /= "000000000000000") then
				next_state <= FALSE_CAKAJ;
			end if;
		
--  -------------------------------------------------------------------
		when PRINT_OK_MSG =>
			next_state <= PRINT_OK_MSG;
			if (CNT_OF = '1') then
				next_state <= FINISH;
			end if;
--  -------------------------------------------------------------------
		when FALSE_CAKAJ =>
			next_state <= FALSE_CAKAJ;
			if (KEY(15) = '1') then
				next_state <= PRINT_ODEPREN; 
			end if;
	  
--  -------------------------------------------------------------------
		when PRINT_ODEPREN =>
			next_state <= PRINT_ODEPREN;
			if (CNT_OF = '1') then
				next_state <= FINISH;
			end if;
--  ----------------------FINISH  a pusti odznovu -----------------------------------------
		when FINISH =>
			next_state <= FINISH;
			if (KEY(15) = '1') then
				next_state <= TST1; 
			end if;
--  -------------------------------------------------------------------
   when others =>
      next_state <= TST1;
   end case;
end process next_state_logic;

--  -------------------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
--  -------------------------------------------------------------------
    when PRINT_ODEPREN =>
		FSM_CNT_CE     <= '1';
		FSM_MX_LCD     <= '1';
		FSM_LCD_WR     <= '1';
		
--  -------------------------------------------------------------------
	when PRINT_OK_MSG =>
		FSM_MX_MEM     <= '1';
		FSM_CNT_CE     <= '1';
		FSM_MX_LCD     <= '1';
		FSM_LCD_WR     <= '1';
	  
--  -------------------------------------------------------------------
    when FINISH =>
		if (KEY(15) = '1') then
			FSM_LCD_CLR    <= '1';
		end if;
--  -------------------------------------------------------------------
	when others =>
		if (KEY(15) = '1') then
			FSM_LCD_CLR    <= '1';
		end if;
		
		if (KEY(14 downto 0) /= "000000000000000") then
			FSM_LCD_WR     <= '1';
		end if;
	  
   end case;
end process output_logic;

end architecture behavioral;

