library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.fixed_float_types.all;
use IEEE.fixed_pkg.all;

entity mash is
    port(
        data_in : in  sfixed(4 downto -11); 
        reset, clk: in  std_logic;
        data_out: out sfixed(4 downto 0)); 
end mash;

architecture rtl of mash is
    signal w1d : sfixed(4 downto  -7) := to_sfixed(0,4,-7);    -- ac_fixed<12,5>
    signal w2d : sfixed(4 downto -11) := to_sfixed(0,4,-11);   -- ac_fixed<16,5>
    signal y1d : sfixed(4 downto   0) := to_sfixed(0,4,0);     -- ac_fixed<5,5>
    signal v1  : sfixed(4 downto   0) := to_sfixed(0,4,0);     -- ac_fixed<5,5>
    signal y2d : sfixed(4 downto   0) := to_sfixed(0,4,0);     -- ac_fixed<5,5>
    signal v2d : sfixed(4 downto   0) := to_sfixed(0,4,0);     -- ac_fixed<5,5>
    signal vsum: sfixed(4 downto   0) := to_sfixed(0,4,0);     -- ac_fixed<5,5>

    function presize(
        outtype:UNRESOLVED_sfixed;
        arg:UNRESOLVED_sfixed)
        return UNRESOLVED_sfixed is
    begin 
        return resize(  arg,
                        outtype'high,
                        outtype'low,
                        fixed_saturate,
                        fixed_truncate
                     );
    end;

    function quantize(arg : UNRESOLVED_sfixed) return UNRESOLVED_sfixed is
        variable t : sfixed(4 downto 0);
    begin
        t := resize(arg            => arg,
                    left_index     => 4,
                    right_index    => 0,
                    overflow_style => fixed_saturate,
                    round_style    => fixed_truncate);
        if t(0) = '0' then
            t := resize(arg            => t + to_sfixed(1,4,0),
                        left_index     => 4,
                        right_index    => 0,
                        overflow_style => fixed_saturate,
                        round_style    => fixed_truncate);
        end if;
        return t;
    end function quantize;

begin
    
    process(all)
        variable uin : sfixed(4 downto -11);   -- ac_fixed<16,5>
        variable y1  : sfixed(4 downto   0);   -- ac_fixed<5,5>
        variable e1  : sfixed(0 downto -11);   -- ac_fixed<12,1>
        variable w1  : sfixed(4 downto  -7);   -- ac_fixed<12,5>
        variable y2  : sfixed(4 downto   0);   -- ac_fixed<5,5>
        variable e2  : sfixed(0 downto -11);   -- ac_fixed<12,1>
        variable w2  : sfixed(4 downto -11);   -- ac_fixed<16,5>
        variable v2  : sfixed(4 downto   0);   -- ac_fixed<5,5>
    begin
        if reset = '1' then
            w1d <= to_sfixed(0,w1d);
            w2d <= to_sfixed(0,w2d);
            y1d <= to_sfixed(0,y1d);
            v1  <= to_sfixed(0,v1);
            y2d <= to_sfixed(0,y2d);
            v2d <= to_sfixed(0,v2d);

        elsif rising_edge(clk) then
            uin := data_in;

            -- ---------------- stepen 1 ----------------
            y1 := quantize(w1d);                
            e1 := presize(e1, w1d - y1);         
            w1 := presize(w1, uin + e1);         

            -- ---------------- stepen 2 ----------------
            y2 := quantize(w2d);
            e2 := presize(e2, w2d - y2);
            w2 := presize(w2, e1 + e2);

            -- ------------------- H2 -------------------
            v2 := presize(v2, y2 - y2d);

            -- ---------------- registri ----------------
            v1  <= y1d;                     
            y1d <= y1;
            w1d <= w1;
            w2d <= w2;
            y2d <= y2;
            v2d <= v2;
        end if;
    end process;

    vsum     <= presize(vsum, v1 + v2d);
    data_out <= vsum;

end rtl;