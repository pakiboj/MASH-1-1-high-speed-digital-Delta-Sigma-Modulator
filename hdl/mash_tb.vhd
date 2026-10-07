library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.Numeric_Std.all;
use IEEE.fixed_float_types.all;
use IEEE.fixed_pkg.all;
use STD.textio.all;

entity mash_tb is
end;

architecture bench of mash_tb is

    component mash
        Port(
            data_in:in sfixed(4 downto -11);
            reset,clk:in STD_LOGIC;
            data_out:out sfixed(4 downto 0)
            );
    end component;
    
    file infile  : text open read_mode is "Podaci";
    file outfile : text open write_mode is "Obradjeni";
    signal data_in:sfixed(4 downto -11);
    signal reset,clk:std_logic;
    signal data_out:sfixed(4 downto 0);
    constant clock_period:time:=10 ns;
    signal stop_the_clock:boolean:=false;
    signal done  :boolean:=false; 

begin
    uut:MASH port map(
                        data_in=>data_in,
                        reset=>reset,
                        clk=>clk,
                        data_out=>data_out
                    );

    stimulus: process
    begin
        reset <= '1';
        wait for clock_period;
        reset <= '0';
        wait for clock_period;
        wait;
    end process;

    stim_proc : process
        variable L : line;
        variable d : real;
    begin
        data_in <= to_sfixed(0, data_in);
        wait until reset = '0';              
        while not endfile(infile) loop
            readline(infile, L);
            read(L, d);
            data_in <= to_sfixed(d, data_in, fixed_saturate, fixed_truncate);
            wait until rising_edge(clk);      
        end loop;
        done <= true;    
        wait;
    end process;

    write_proc : process
        variable L : line;
    begin
        wait until reset = '0';
        loop
            wait until rising_edge(clk);
            wait for 1 ns;              
            write(L, to_integer(data_out));
            writeline(outfile, L);
            exit when done;          
        end loop;
        stop_the_clock <= true;      
        file_close(infile);
        file_close(outfile);
        wait;
    end process; 

    clocking : process
        begin
            while not stop_the_clock loop
                clk<='0','1' after clock_period/2;
                wait for clock_period;
            end loop;
            wait;
        end process;
end;

-- ghdl -a --std=08 *.vhd && ghdl --elab-run --std=08 mash_tb --wave=mash.ghw

-- ghdl -a --std=08 *.vhd
-- ghdl --elab-run --std=08 mash_tb --wave=mash.ghw
-- gtkwave mash.ghw &

-- gtkwave mash.ghw mash.gtkw