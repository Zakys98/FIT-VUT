-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2019 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): xzakji02
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

	signal PC_int : std_logic;
	signal PC_dec : std_logic;
	signal PC_out : std_logic_vector(12 downto 0);
	
	signal PTR_int : std_logic;
	signal PTR_dec : std_logic;
	signal PTR_out : std_logic_vector(12 downto 0);

	signal CNT_int : std_logic;
	signal CNT_dec : std_logic;
	signal CNT_reset : std_logic := '0';
	signal CNT_out : std_logic_vector(7 downto 0);


	signal MX1_sel	: std_logic_vector(0 downto 0);

	signal MX2_sel	: std_logic_vector(0 downto 0);
	signal MX2_out	: std_logic_vector(12 downto 0);

	signal MX3_sel	: std_logic_vector(1 downto 0);

	type input_char is (fetch, decode,
				  ptrInc, ptrSub, 
				  valIncR, valIncR2, valSubR, valSubR2, 
				  whileSR, whileSR2, whileSR3, whileSR4, 
				  whileER, whileER2, whileER3, whileER4, whileER5, whileER6,  
				  printR, printR2, scanR, 
				  toTmpR, toTmpR2, fromTmp, fromTmp2, Eof, comment);

    signal next_state    : input_char;
    signal present_state : input_char;

begin

 CNT: process(CLK,CNT_int, CNT_dec, RESET)--obmedzene na 8bitov
    begin
        if (RESET = '1') then
            CNT_out <= "00000000";
	  elsif (CLK'event) and (CLK = '1') then
			if(CNT_int = '1') then
				CNT_out <= CNT_out + 1;
			elsif(CNT_dec = '1') then
				CNT_out <= CNT_out - 1;
			elsif(CNT_reset = '1') then
				CNT_out <= "00000001";
			end if;
		end if;
	end process;

PC: process(CLK,PC_int,PC_dec,RESET)
	begin
		if (RESET = '1') then
			PC_out <= (others => '0');
		elsif (CLK'event) and (CLK = '1') then
			if(PC_int = '0' and PC_dec = '1') then
				PC_out <= PC_out - 1;
			elsif( PC_int = '1' and PC_dec = '0') then
				PC_out <= PC_out + 1;
			end if;
		end if;
	end process;

PTR: process(CLK,PTR_int, PTR_dec, RESET)--ukazatel do pameti dat 
	begin
		if (RESET = '1') then
			PTR_out <= "1000000000000";
		elsif (CLK'event) and (CLK = '1') then
			if(PTR_int = '0' and PTR_dec = '1') then
					if (PTR_out = "1000000000000")then
						PTR_out <= "1111111111111";
					else 
						PTR_out <= PTR_out - 1;
					end if;
			elsif(PTR_int = '1' and PTR_dec = '0') then
				if (PTR_out = "1111111111111") then
					PTR_out <= "1000000000000";
				else
					PTR_out <= PTR_out + 1;
				end if;
            	end if;
		end if;
	end process;

mx2: process (MX2_sel, PTR_out)
begin
	if (MX2_sel = "0") then
		MX2_out <= PTR_out;
	else
		MX2_out <= "1000000000000";
	end if;
end process;

mx1: process (MX1_sel, MX2_out, PC_out)
begin
	if (MX1_sel = "0") then
		DATA_ADDR <= PC_out;
	else
		DATA_ADDR <= MX2_out;
	end if;
end process;

mx3: process (MX3_sel, IN_DATA, DATA_RDATA)
 begin
   if MX3_sel= "00" then
     DATA_WDATA <= IN_DATA;
   elsif MX3_sel= "01" then
     DATA_WDATA <= DATA_RDATA - 1;
   elsif MX3_sel= "10" then
     DATA_WDATA <= DATA_RDATA + 1;
   elsif MX3_sel= "11" then
     DATA_WDATA <= DATA_RDATA;
   end if;
 end process;

fsm_start: process(CLK, RESET, EN, next_state)
    begin
        if (RESET = '1') then
            present_state <= fetch;
        elsif (EN = '1' and CLK'event and CLK = '1') then
            present_state <= next_state;
        end if;
end process;


next_state_logic: process(present_state, DATA_RDATA, OUT_BUSY, IN_VLD, CNT_out, EN)
	begin

        PTR_int <= '0';
        PTR_dec <= '0';
        PC_int  <= '0';
        PC_dec  <= '0';
        CNT_int <= '0';
        CNT_dec <= '0';
	  CNT_reset <= '0'; 
	  IN_REQ  <= '0';
        OUT_WE  <= '0';
	  OUT_DATA <= DATA_RDATA;
        DATA_EN <= '0';
	  MX1_sel <= "0";
	  MX2_sel <= "0";
	  MX3_sel <= "00";

	  case (present_state) is
            when fetch =>
                DATA_EN    <= '1';
                DATA_RDWR  <= '0';
		    MX1_sel <= "0";
                next_state <= decode;
            when decode =>
                case DATA_RDATA is
			        when X"3E"  =>  next_state <= ptrInc;  -- > DONE
			        when X"3C"  =>  next_state <= ptrSub;  -- < DONE
			        when X"2B"  =>  next_state <= valIncR; -- + DONE
			        when X"2D"  =>  next_state <= valSubR; -- - DONE
			        when X"5B"  =>  next_state <= whileSR;  -- [ 
			        when X"5D"  =>  next_state <= whileER;  -- ]
			        when X"2E"  =>  next_state <= printR;  -- . DONE
			        when X"2C"  =>  next_state <= scanR;   -- , DONE
			        when X"24"  =>  next_state <= toTmpR;   -- $ DONE
			        when X"21"  =>  next_state <= fromTmp; -- !  DONE
			        when X"00"  =>  next_state <= Eof;     -- null DONE
                    when others =>  next_state <= comment; --komentar DONE
                end case;
            when ptrInc =>
                	PTR_int    <= '1';
		    	PTR_dec    <= '0';
                	PC_int     <= '1';	
		    	PC_dec     <= '0';
                	next_state <= fetch;
		when ptrSub =>
		    	PTR_int    <= '0';
                	PTR_dec    <= '1';
                	PC_int     <= '1';
		    	PC_dec     <= '0';
          		next_state <= fetch;
		when valIncR =>
			MX1_sel <= "1";
			MX2_sel <= "0";
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			next_state <= valIncR2;
		when valIncR2 =>
			MX1_sel <= "1";
			MX2_sel <= "0";
			MX3_sel <= "10";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			PC_int <= '1';
			PC_dec <= '0';
			next_state <= fetch;
		when valSubR =>
			MX1_sel <= "1";
			MX2_sel <= "0";
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			next_state <= valSubR2;
		when valSubR2 =>
			MX1_sel <= "1";
			MX2_sel <= "0";
			MX3_sel <= "01";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			PC_int <= '1';
			PC_dec <= '0';
			next_state <= fetch;
		when printR =>
			if (OUT_BUSY = '1') then
				next_state <= printR;
			else
				MX1_sel <= "1";
				MX2_sel <= "0";
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				next_state <= printR2;
			end if;
		when printR2 =>
			OUT_WE <= '1';
            	OUT_DATA <= DATA_RDATA;
            	PC_int <= '1';
			PC_dec <= '0';
            	next_state <= fetch;

		when scanR =>
			IN_REQ <= '1';
			if (IN_VLD = '0') then
				next_state <= scanR;
			else
				MX1_sel <= "1";
				MX2_sel <= "0";
				MX3_sel <= "00";
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				PC_int <= '1';
				PC_dec <= '0';
				next_state <= fetch;
			end if;

		when whileSR =>
			MX1_sel <= "1";
			MX2_sel <= "0";
			PC_int <= '1';
			PC_dec <= '0';
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			next_state <= whileSR2;
		when whileSR2 =>
			if (DATA_RDATA = "00000000") then
				CNT_reset <= '1';
				next_state <= whileSR3;
			else
				next_state <= fetch;
			end if;
		when whileSR3 =>
			if(CNT_out = "00000000") then
				next_state <= fetch;
			else
				MX1_sel <= "0";
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				next_state <= whileSR4;
			end if;
		when whileSR4 =>
			if(DATA_RDATA = X"5B") then
				CNT_int <= '1';
				CNT_dec <= '0';
			elsif(DATA_RDATA = X"5D") then
				CNT_int <= '0';
				CNT_dec <= '1';
			end if;
			PC_int <= '1';
			PC_dec <= '0';
			next_state <= whileSR3;

		when whileER =>
			MX1_sel <= "1";
			MX2_sel <= "0";
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			next_state <= whileER2;
		when whileER2=>
			if (DATA_RDATA = "00000000") then
				PC_int <= '1';
				PC_dec <= '0';
				next_state <= fetch;
			else
				next_state <= whileER3;
			end if;
		when whileER3 =>
			PC_int <= '0';
			PC_dec <= '1';
			CNT_reset <= '1';
			next_state <= whileER4;
		when whileER4 =>
			if(CNT_out = "00000000") then
				next_state <= fetch;
			else
				MX1_sel <= "0";
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				next_state <= whileER5;
			end if;
		when whileER5 =>
			if(DATA_RDATA = X"5D") then
				CNT_int <= '1';
				CNT_dec <= '0';
			elsif(DATA_RDaTA = X"5B") then
				CNT_int <= '0';
				CNT_dec <= '1';
			end if;
			next_state <= whileER6;
		when whileER6 =>
			if(CNT_out = "00000000") then
				PC_int <= '1';
				PC_dec <= '0';
			else
				PC_int <= '0';
				PC_dec <= '1';
			end if;
			next_state <= whileER4;
			
		when toTmpR =>
			MX1_sel <= "1";
			MX2_sel <= "0";
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			next_state <= toTmpR2;
		when toTmpR2 =>
			MX1_sel <= "1";
			MX2_sel <= "1";
			MX3_sel <= "11";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			PC_int <= '1';
			PC_dec <= '0';
			next_state <= fetch;

		when fromTmp =>
			MX1_sel <= "1";
			MX2_sel <= "1";
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			next_state <= fromTmp2;
		when fromTmp2 =>
			MX1_sel <= "1";
			MX2_sel <= "0";
			MX3_sel <= "11";
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			PC_int <= '1';
			PC_dec <= '0';
			next_state <= fetch;		

            when comment =>
                	next_state <= fetch;
		    	PC_int <='1';
                	PC_dec <='0';
            when Eof =>
                	next_state <= Eof;
            when others=>
                	next_state <= fetch;

        end case;
    end process;
 
end behavioral;