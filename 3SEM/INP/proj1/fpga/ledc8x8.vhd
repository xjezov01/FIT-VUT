-- INP 1.projekt 21.10.2014
-- Filip Ježovica
-- xjezov01
-- encoding: UTF-8

--libs
library ieee;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;

-- entity
entity ledc8x8 is
  port (
	SMCLK: in std_logic;
	RESET: in std_logic;
	LED: out std_logic_vector(0 to 7);
	ROW: out std_logic_vector(0 to 7)
  );
end entity; -- ledc8x8

--architecture
architecture display_name of ledc8x8 is

	signal row_count: std_logic_vector(7 downto 0);	-- citac pre riadky
	signal ce_count: std_logic_vector(7 downto 0);	-- pomocný čítač pre generovanie signálu ENABLE
	signal ce: std_logic;	-- signál clock enable

begin

	-- čítač pre zníženie frekvencie SMCLK/256
    ce_gen: process(SMCLK, RESET)
    begin
    	if RESET = '1' then	-- asynchronny reset
    		ce_count <= X"00";	-- vynuluj čítač /X - hexa/
        elsif SMCLK'event and SMCLK = '1' then	-- v okamihu nástupnej hrany SMCLK
            ce_count <= ce_count + 1;
        end if;
    end process ce_gen;

    ce <= '1' when ce_count = X"FF" else '0';	-- ce = SMCLK/256


    -- rotačný register na rotáciu riadkov
    row_rotate : process( RESET, SMCLK, ce )
    begin
    	if RESET = '1' then -- asynchronny reset
    		row_count <= "10000000"; -- nastavíme počiatočný riadok
    	elsif SMCLK'event and SMCLK = '1' and ce = '1' then -- v okamihu nástupnej hrany a sig ENABLE
    		row_count <= row_count(0) & row_count(7 downto 1); -- touto konkatenáciou mozme rotovat po 1 bite
    	end if ;
    	
    end process ; -- row_rotate


    -- dekodér riadkov , LED može ísť priamo (OUT) iba zapisujeme, nečítame
    rows_decode : process( row_count )
    begin
    	case row_count is								-- F J
			when "10000000" => LED <= "00010000";
			when "01000000" => LED <= "01111110";
			when "00100000" => LED <= "01111110";
			when "00010000" => LED <= "00011110";
			when "00001000" => LED <= "01111110";
			when "00000100" => LED <= "01101110";
			when "00000010" => LED <= "01110101";
			when "00000001" => LED <= "01111011";

			when others =>     LED <= "11111111"; -- keby keby, vypni ledky
		end case;
    	
    end process ; -- rows_decode

    -- aktivátor portov
    ROW <= row_count;

end architecture; -- display_name
