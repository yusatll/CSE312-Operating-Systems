/* SPIM S20 MIPS simulator.
   Execute SPIM syscalls, both in simulator and bare mode.
   Execute MIPS syscalls in bare mode, when running on MIPS systems.
   Copyright (c) 1990-2010, James R. Larus.
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

   Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

   Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation and/or
   other materials provided with the distribution.

   Neither the name of the James R. Larus nor the names of its contributors may be
   used to endorse or promote products derived from this software without specific
   prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef _WIN32
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdexcept>

#ifdef _WIN32
#include <io.h>
#endif

#include "spim.h"
#include "string-stream.h"
#include "inst.h"
#include "reg.h"
#include "mem.h"
#include "sym-tbl.h"
#include "syscall.h"

#include <iostream>
using namespace std;
mem_addr kernel_PC;
int pali_counter = 0;
#define ilerle  4

int pali_index_1 = 0;
int pali_index_2 = 0;
int pali_index_3 = 0;
int pali_index_4 = 0;
int pali_index_5 = 0;
int pal_index = 0;

#ifdef _WIN32
/* Windows has an handler that is invoked when an invalid argument is passed to a system
   call. https://msdn.microsoft.com/en-us/library/a9yf33zb(v=vs.110).aspx

   All good, except that the handler tries to invoke Watson and then kill spim with an exception.

   Override the handler to just report an error.
*/

#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <stdlib.h>
#include <time.h>


void myInvalidParameterHandler(const wchar_t* expression,
   const wchar_t* function, 
   const wchar_t* file, 
   unsigned int line, 
   uintptr_t pReserved)
{
  if (function != NULL)
    {
      run_error ("Bad parameter to system call: %s\n", function);
    }
  else
    {
      run_error ("Bad parameter to system call\n");
    }
}

static _invalid_parameter_handler oldHandler;

void windowsParameterHandlingControl(int flag )
{
  static _invalid_parameter_handler oldHandler;
  static _invalid_parameter_handler newHandler = myInvalidParameterHandler;

  if (flag == 0)
    {
      oldHandler = _set_invalid_parameter_handler(newHandler);
      _CrtSetReportMode(_CRT_ASSERT, 0); // Disable the message box for assertions.
    }
  else
    {
      newHandler = _set_invalid_parameter_handler(oldHandler);
      _CrtSetReportMode(_CRT_ASSERT, 1);  // Enable the message box for assertions.
    }
}
#endif

void pc_count()
{
  R[REG_A3] = PC + ilerle;
}

void print_timer()
{
  cout << endl <<"TIMER HANDLER ";  
  /*
  cout << "microkernel : " << hex << kernel_PC << endl;
  cout << "program : " << hex << PC << endl;
  cout << endl;
  */
  pc_count();
  cout << "- INTERRUPT \n";
}



/*You implement your handler here*/
void SPIM_timerHandler()
{
  print_timer();
  //cout << "ProcessID              -> " << R[8] << endl;
  
  running_prog(kernel_PC);

}
/* Decides which syscall to execute or simulate.  Returns zero upon
   exit syscall and non-zero to continue execution. */
bool flag = false;


void running_prog(mem_addr pc_param)
{
  bool x;
  run_program(pc_param, DEFAULT_RUN_STEPS , false, false, &x);
}

void process_info()
{
  cout << endl;
  cout << "PROCESS INFO:\n";
  char* name = (char *)mem_reference (R[27]);   // REG_K1
  long sp = R[29];
  // name = reg k1
  // PC = A3
  // ProcessID = T0 = R[8]
  cout << "ProcessID              -> " << R[8] << endl;
  cout << "Process Name           -> " << name << endl;
  cout << "Program Counter        -> " << R[REG_A3] << endl;
  cout << "Stack Pointer Address  -> " << sp << endl;
  cout << endl;
  pal_index = R[8];

}

void run_syscall_prog()
{
  char* file = (char *)mem_reference (R[REG_A0]);
  read_assembly_file(file);
  PC = PC + ilerle;


  kernel_PC = PC;
  // take PC
  R[REG_A3] = find_symbol_address(file);

  cout << "PC: " << R[REG_A3] << endl;
  //run_program (kernel_PC, DEFAULT_RUN_STEPS , false, false, &x);
  running_prog(kernel_PC);
}

void close_programs()
{
  bool booltrue = true;
  char *nullptr = NULL;
  initialize_world(nullptr,booltrue);
}


int do_syscall ()
{
#ifdef _WIN32
    windowsParameterHandlingControl(0);
#endif

  /* Syscalls for the source-language version of SPIM.  These are easier to
     use than the real syscall and are portable to non-MIPS operating
     systems. */

  switch (R[REG_V0])
    {
    case PRINT_INT_SYSCALL:
    {

      write_output (console_out, "%d", R[REG_A0]);
      break;
    }

    case PRINT_FLOAT_SYSCALL:
      {
	float val = FPR_S (REG_FA0);

	write_output (console_out, "%.8f", val);
	break;
      }

    case PRINT_DOUBLE_SYSCALL:
      write_output (console_out, "%.18g", FPR[REG_FA0 / 2]);
      break;

    case PRINT_STRING_SYSCALL:
      write_output (console_out, "%s", mem_reference (R[REG_A0]));
      break;

    case READ_INT_SYSCALL:
      {
    	static char str [256];

    	read_input (str, 256);
    	R[REG_RES] = atol (str);
    	break;
      }


    case READ_FLOAT_SYSCALL:
      {
	static char str [256];

	read_input (str, 256);
	FPR_S (REG_FRES) = (float) atof (str);
	break;
      }

    case READ_DOUBLE_SYSCALL:
      {
	static char str [256];

	read_input (str, 256);
	FPR [REG_FRES] = atof (str);
	break;
      }

    case READ_STRING_SYSCALL:
      {
	read_input ( (char *) mem_reference (R[REG_A0]), R[REG_A1]);
	data_modified = true;
	break;
      }

    case SBRK_SYSCALL:
      {
	mem_addr x = data_top;
	expand_data (R[REG_A0]);
	R[REG_RES] = x;
	data_modified = true;
	break;
      }

    case PRINT_CHARACTER_SYSCALL:
      write_output (console_out, "%c", R[REG_A0]);
      break;

    case READ_CHARACTER_SYSCALL:
      {
	static char str [2];

	read_input (str, 2);
	if (*str == '\0') *str = '\n';      /* makes xspim = spim */
	R[REG_RES] = (long) str[0];
	break;
      }

    case EXIT_SYSCALL:
      spim_return_value = 0;
      return (0);

    case EXIT2_SYSCALL:
      spim_return_value = R[REG_A0];	/* value passed to spim's exit() call */
      return (0);

    case OPEN_SYSCALL:
      {
#ifdef _WIN32
        R[REG_RES] = _open((char*)mem_reference (R[REG_A0]), R[REG_A1], R[REG_A2]);
#else
	R[REG_RES] = open((char*)mem_reference (R[REG_A0]), R[REG_A1], R[REG_A2]);
#endif
	break;
      }

    case READ_SYSCALL:
      {
	/* Test if address is valid */
	(void)mem_reference (R[REG_A1] + R[REG_A2] - 1);
#ifdef _WIN32
	R[REG_RES] = _read(R[REG_A0], mem_reference (R[REG_A1]), R[REG_A2]);
#else
	R[REG_RES] = read(R[REG_A0], mem_reference (R[REG_A1]), R[REG_A2]);
#endif
	data_modified = true;
	break;
      }

    case WRITE_SYSCALL:
      {
	/* Test if address is valid */
	(void)mem_reference (R[REG_A1] + R[REG_A2] - 1);
#ifdef _WIN32
	R[REG_RES] = _write(R[REG_A0], mem_reference (R[REG_A1]), R[REG_A2]);
#else
	R[REG_RES] = write(R[REG_A0], mem_reference (R[REG_A1]), R[REG_A2]);
#endif
	break;
      }

    case CLOSE_SYSCALL:
      {
#ifdef _WIN32
	R[REG_RES] = _close(R[REG_A0]);
#else
	R[REG_RES] = close(R[REG_A0]);
#endif
	break;
      }



    // 19
    case RUN_MK_SYS:
    {
      kernel_PC = PC + ilerle;
      //run_program (R[REG_A3], DEFAULT_RUN_STEPS , false, false, &x);
      running_prog(R[REG_A3]);
      break;
    }

    // 40
    case EXIT_PROGRAMS:
    {
      char* file_running_prog_name = (char *)mem_reference (R[REG_A0]);
      cout << "ASSMEBLY - FILE - FINISH: " << file_running_prog_name << endl;
      //run_program (kernel_PC, DEFAULT_RUN_STEPS , false, false, &x);
      running_prog(kernel_PC);
      break;
    }

    // 21
    case EXIT_SPIMOS:
    {
        close_programs();
        exit(0);
        break;
    }

      // 22
      case WRITE_INFO:
      {
        process_info();
        break;
      }

      // 23
      case 23:
      {
        int integer;
        scanf("%d",&integer);
        // t6 okunan değerler.
        R[14] = integer;
        break;
      }

      // 24
      case 24:
      {
        /*
        char input [256];

        read_input (input, 256);

        long * pa = (long *)mem_reference(R[REG_A0]);
        sscanf(input,"%ld",pa);
        */
        break;
        
      }
      // 25
      case RANDOM_NUM_SYS:
      {
        srand (time(NULL));
        int run1 = rand() % 4;
        int run2 = rand() % 4;
        int run3 = rand() % 4;

        while(run1 == run2 || run2 == run3 || run1 == run3){
            run1 = rand() % 4;
            run2 = rand() % 4;
            run3 = rand() % 4;
            
            //printf("run1: %d, run2: %d, run3: %d\n",run1, run2, run3);
        }


        R[22] = run1;   // s6 
        R[23] = run2;   // s7
        R[24] = run3;   // t8
        
        // s3
        if(run1 == 0){
          cout << "First Program is Linear Search\n";
        }
        else if(run1 == 1){
          cout << "First Program is Binary Search\n";
        }
        else if(run1 == 2){
          cout << "First Program is Collatz\n";
        }
        else if(run1 == 3){
          cout << "First Program is Palindrome\n";
        }

        // s4
        if(run2 == 0){
          cout << "Second Program is Linear Search\n";
        }
        else if(run2 == 1){
          cout << "Second Program is Binary Search\n";
        }
        else if(run2 == 2){
          cout << "Second Program is Collatz\n";
        }
        else if(run2 == 3){
          cout << "Second Program is Palindrome\n";
        }

        // t4
        if(run3 == 0){
          cout << "Third Program is Linear Search\n";
        }
        else if(run3 == 1){
          cout << "Third Program is Binary Search\n";
        }
        else if(run3 == 2){
          cout << "Third Program is Collatz\n";
        }
        else if(run3 == 3){
          cout << "Third Program is Palindrome\n";
        }

        break;
      }

      // 26
      case PALI_DICT:
      {
        //cout << "PALI_DICT:: \n";
        //pal_index = R[8];
        //cout << "REG: T8: " << R[24] << " - T9: " << R[25] << endl;
        //pali_counter = R[24];
        //cout << "PALİ COUNTER: -> " <<pali_counter << endl; 
          char * dic[100] = {"aba", "how", "book", "ada", "ahmet", "yusa", "gebze", "saas", "write", "read",
                      "java", "python", "yusasuy", "embedded", "real", "computer", "engineer", "word",
                      "spim", "mips", "hello", "world", "english", "russian", "lamba", "phone", "google", 
                      "process", "table", "thread", "anna", "stm", "geekday", "pycharm", "keras", "pytorch",
                      "pen", "pencil", "teataet", "ova", "yayla", "deniz", "sea", "oceans", "whatsapp",
                      "twitter", "facebook", "feef" "ali", "veli", "okul", "system", "palilap", 
                      "name", "football", "basketball", "sublime", "glass", "box", "wiki", 
                      "buggs", "kolilok" "teams", "mine", "your", "buzzer", "jumper", "led", "rgb",
                      "sensor","sound", "big", "small", "very", "good", "bad", "refer",
                      "assembly", "string", "ubuntu", "kali", "tea", "coffee", "apple", "kotlin",
                      "baby", "palindrome", "algorithm", "binary", "linear", "search", "collatz", 
                      "asdsa", "mouse", "cup", "charger","hahahah", "alti", "yedi", "sekiz" ,
                      "dokuz" ,"last"};

        if (pal_index == 1)
        {
          pali_counter = pali_index_1;
        }
        else if (pal_index == 2)
        {
          pali_counter = pali_index_2;
        }
        else if (pal_index == 3)
        {
          pali_counter = pali_index_3;
        }
        else if (pal_index == 4)
        {
          pali_counter = pali_index_4;
        }
        else if (pal_index == 5)
        {
          pali_counter = pali_index_5;
        }

        pali_counter = R[24]; // t8

        if (pali_counter == 100)
        {
          goto read2;
        }
        
        //cout << "DİC: T8: " << pali_counter << endl;
        char * rega0 = (char *)mem_reference (R[REG_A0]);

        int i=0;
        for (i = 0; i < strlen(dic[pali_counter]); ++i)
        {
          if(dic[pali_counter][i] != '\0' && dic[pali_counter][i] != '\n') {
            rega0[i] = dic[pali_counter][i];

          }
        }
        rega0[i] = '\0';

        //cout << "rega0: "<< rega0 << endl;
        pali_counter++;
        cout << pali_counter << " ";

        if (pal_index == 1)
        {
          pali_index_1 = pali_counter;
        }
        else if (pal_index == 2)
        {
          pali_index_2 = pali_counter;
        }
        else if (pal_index == 3)
        {
          pali_index_3 = pali_counter;
        }
        else if (pal_index == 4)
        {
          pali_index_4 = pali_counter;
        }
        else if (pal_index == 5)
        {
          pali_index_5 = pali_counter;
        }

        break;
      }

      // 27 
    case YD:
    {
      read2:
      //cout << "READ 2 -> REG: T8: " << R[24] << " - T9: " << R[25] << endl;
      int i=0;
      char my_inpit[255] = " ";
      cout << "Enter a string for Palindrome : \n";
      cin >> my_inpit;
      //cout << "my_inpit: " << my_inpit << endl;
      char * my_inpit1 = "";
      my_inpit1 = (char *) mem_reference (R[REG_A0]);
      for (i = 0; i < strlen(my_inpit); ++i)
      {
        if(my_inpit[i] != '\0' && my_inpit[i] != '\n') {
          if(my_inpit[i] < 97){ 
            my_inpit[i] += 32;
          }
          my_inpit1[i] = my_inpit[i];
        }
      }
      my_inpit1[i] = '\0';
      //cout << "my_inpit1: " << my_inpit1  << strlen(my_inpit1)<< endl;
      R[REG_A1] = strlen(my_inpit1);
      break;
    }

    // 28
    case pali_result:
    {
      again_ask:
      //cout << "REG: T8: " << R[24] << " - T9: " << R[25] << endl;
      cout << "Do you want to continue (y/n)?\n";
      char c;
      cin >> c;

      if(c == 'y' || c == 'Y')
      {
        R[REG_A1] = 1;

      }
      else if (c == 'n' || c== 'N')
      {
        R[REG_A1] = 2;

        cout << "Good bye.\n";
      }
      else
        goto again_ask;

      //cout << "reg a1:" << R[REG_A1] << endl;
      break;
    }

    // 29
    case random_one_program:
    {
      srand (time(NULL));
      int prog = rand() % 4;
      //R[22] = prog;   // s6

      char* filename;
      //cout << "num: " << prog << endl; 
      if(prog == 0)
        filename = "LinearSearch.asm";
      else if(prog == 1)
        filename = "BinarySearch.asm";
      else if(prog == 2)
        filename = "Collatz.asm";
      else if(prog == 3)
        filename = "Palindrome.asm";

      //filename = "Palindrome.asm";//  sil
      //cout << "29 START: " << filename << endl;

      char * rega0 = (char *)mem_reference (R[REG_A0]);

      int i=0;
      for (i = 0; i < strlen(filename); ++i)
      {
        rega0[i] = filename[i];
      }
      rega0[i] = '\0';


      run_syscall_prog();


      break;
    }

          // 30
    case MK_SYS:
    {
        run_syscall_prog();
        break;
    }

    default:
      run_error ("Unknown system call: %d\n", R[REG_V0]);
      break;
    }



#ifdef _WIN32
    windowsParameterHandlingControl(1);
#endif
  return (1);
}




void
handle_exception ()
{
  if (!quiet && CP0_ExCode != ExcCode_Int)
    error ("Exception occurred at PC=0x%08x\n", CP0_EPC);

  exception_occurred = 0;
  PC = EXCEPTION_ADDR;

  switch (CP0_ExCode)
    {
    case ExcCode_Int:
      break;

    case ExcCode_AdEL:
      if (!quiet)
	error ("  Unaligned address in inst/data fetch: 0x%08x\n", CP0_BadVAddr);
      break;

    case ExcCode_AdES:
      if (!quiet)
	error ("  Unaligned address in store: 0x%08x\n", CP0_BadVAddr);
      break;

    case ExcCode_IBE:
      if (!quiet)
	error ("  Bad address in text read: 0x%08x\n", CP0_BadVAddr);
      break;

    case ExcCode_DBE:
      if (!quiet)
	error ("  Bad address in data/stack read: 0x%08x\n", CP0_BadVAddr);
      break;

    case ExcCode_Sys:
      if (!quiet)
	error ("  Error in syscall\n");
      break;

    case ExcCode_Bp:
      exception_occurred = 0;
      return;

    case ExcCode_RI:
      if (!quiet)
	error ("  Reserved instruction execution\n");
      break;

    case ExcCode_CpU:
      if (!quiet)
	error ("  Coprocessor unuable\n");
      break;

    case ExcCode_Ov:
      if (!quiet)
	error ("  Arithmetic overflow\n");
      break;

    case ExcCode_Tr:
      if (!quiet)
	error ("  Trap\n");
      break;

    case ExcCode_FPE:
      if (!quiet)
	error ("  Floating point\n");
      break;

    default:
      if (!quiet)
	error ("Unknown exception: %d\n", CP0_ExCode);
      break;
    }
}

void take_an_input(int size, char * string)
{
  read_input(string, size);
}