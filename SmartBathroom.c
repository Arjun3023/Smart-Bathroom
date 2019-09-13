//This Programs Convert Given Assembly code into binary
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
//structure for storing various hash tables
struct mnemonic
{
    char text[20];
    char binary[50];
};
typedef struct mnemonic mnemonic;

mnemonic Opcodes[50];
mnemonic Register[50];
mnemonic Label[50];

//different constants
int instructionLength= 16;      // Our instruction length is 16 bit
int registerLength =5;          // Address is of bit
int labelLength = 5;            // Binary Code of label is 5bit
int dataLength = 10;            // Length of data in R1 and R2



int number_of_op_codes=0,number_of_op_registers=0,number_of_op_labels=0,instLen=0;
char filename[10];
/*
   * This function simply reverses the input string.
*/

void reverse(char *str){
	char temp;
	int i,j;
	int len;
	len = strlen(str);
	for(i=0,j=len-1;i<j;i++,j--)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;

	}

}
/*
   * This function convert decimal into binary 
*/
int binary(int num,int length,char *temp)
{
    int k;
    for(k=0;k<length;k++)
    {
        temp[k]=num%2+'0';
        num>>=1;
    }
    temp[k]='\0';
    reverse(temp);
	return 0;
}


int Check_Pass_1(char s[],int num)
{
    int i=0,j=0,n,w=0,k;
    int u=0;
    int l=0;
    
     int hashcount=0;       //counts the number of hash(#)
    int floatcount=0;         //Counts the number of dots(.)
    int commacount=0;       //Counts the number of comma(;)
    int registercount=0;    //Counts Consecutive of Registers(RR)

    char temp[50];          //Temporary Array

    //Copies the data to temporary array
    while(s[l]!='\0')
    {
    	temp[l] = s[l];
    	l++;
	}

     //Finds the index of space( )

	while(s[i]!=' ')
    {
        i++;
    }
       while (temp[u]!='\0')
    {
    	   /*
                * These utilities count the number of #,.and R
            */
        if(temp[u]=='#') 
		{
		hashcount++;
	}
        else if(temp[u]==',') 
		{
		commacount++;
	}
        else if(temp[u] == '.')
		{
		 floatcount++;
	}
        else if(temp[u]== 'R' && temp[u+1] == 'R') 
		{
			registercount++;
		} //Ifconsecutive two R's 
		u++;
}

    /* Check the syntax of immediate addressing*/

    if (s[i-1]=='I' && s[i+1]!='#')
    {
        printf("Incorrect type of addressing for Immediate function\t Error in Line no: %d\n",num);
        return 0;
    }

	else if (s[i-1]=='I' && s[i+1]=='#' && hashcount==1 && commacount==1 && floatcount==0 && registercount == 0)
    {
    /*
    	printf("hash: %d\n",hashcount);
        printf("comma: %d\n",commacount);
		printf("float: %d\n",floatcount);
		printf("register: %d\n",registercount);

    */
        int len=strlen(s);
        for(k=0;k<=len;k++)
        {
            if(s[k]==' '||s[k]==',')
            {
                w++;
                int q=0;
                if (w==2)
                {
                    n=Convert_Decimal_number(s,j+1,k);
                    break;
                }
                j=i+1;
            }
        }
		//printf("%d\n",n);
        if (Check_Range(n))
            return 1;
        else
        {
            printf("Value to be transferred is out of index\t Error in Line no: %d\n",num);
            return 0;
        }
    }

    else if (s[i-1]=='R' && (s[i+1]!='R' || s[i+5]!='R'))
    {
        printf("Invalid name for a register\t Error in Line no: %d\n",num);
        return 0;
    }
    else if (s[0]=='J' && s[i+1]!='L')
    {
        printf("Jump statement must be followed by a Label name\t Error in Line no: %d\n",num);
        return 0;
    }
    else if(hashcount > 1 || commacount > 1 || floatcount > 1)
            {
            	printf("e no: %d\n",num);
            	return 0;
			}
 //   return 1;
}

int Convert_Decimal_number(char* s,int j,int i)
{
    int num=0,k;
    for(k=j;k<i;k++)
    {
        num=num*10+(s[k]-'0');
    }
    return num;
}
// First Pass will Check Our Assemby Code about Error And Labels And Take CAre OF All This
int firstpass()
{
    int i;
    FILE *f=fopen(filename,"r");
    FILE *l=fopen("labels.txt","w");
    char s[50],lab[10],temp[10];
    int line_cnt=0,count=0;
    while(fgets(s,50,f)!=NULL)
    {
        s[strlen(s)-1]='\0';
        line_cnt++;
        int flag=Check_Pass_1(s,line_cnt); // this checks for errors present in the mnemonic code
        if (flag==0)
            return flag;
        if(s[0]=='L')
        {
            count++;
            lab[0]=s[0];
            lab[1]=s[1];
            if(s[2]==':')
            {
                lab[2]='\0';
            }
            else
            {
                lab[2]=s[2];
                lab[2]='\0';
            }
            binary(line_cnt-(count-1),labelLength,temp);
            fprintf(l,"%s %s\n",lab,temp);
            line_cnt++;
        }
    }
    fclose(f);// First Pass will 
    fclose(l);
    return 1;
}
//function to generate hash table for labels
void Generate_Op_Labels()
{
    number_of_op_labels=0;
    FILE *f=fopen("labels.txt","r");
    if(f==NULL)
    {
        printf("Label Table not found\n");
    }
    else
    {
        while(fscanf(f,"%s %s",Label[number_of_op_labels].text,Label[number_of_op_labels].binary)!=EOF){number_of_op_labels++;}
    }
    fclose(f);
}


void Assemblycode()
{
    FILE *as=fopen(filename,"r");
    char s[50];
    while(fgets(s,50,as)!=NULL)
    {
        printf("%s",s);
    }
    fclose(as);
}

void getRegister()
{
    int i=0;
    for(i=0;i<number_of_op_registers;i++)
    {
        printf("%s %s\n",Register[i].text,Register[i].binary);
    }
}

void getOpcode()
{
    int i=0;
    for(i=0;i<number_of_op_codes;i++)
    {
        printf("%s %s\n",Opcodes[i].text,Opcodes[i].binary);
    }
}

void Machinecode()
{
    FILE *f=fopen("Machine_File.txt","r");
    char s[50];
    while(fgets(s,50,f)!=NULL)
    {
        printf("%s",s);
    }
    fclose(f);
}

void Get_Label()
{
    int i=0;
    for(i=0;i<number_of_op_labels;i++)
    {
        printf("%s %s\n",Label[i].text,Label[i].binary);
    }
}

int Check_Range(int num)
{
    if (num>1023 || num <-1024)
        return 0;
    else
        return 1;
}

//function to convert a number in form of string to binary string
void get_binary(char *s,int l,int h,char *temp)
{
    int num,i;
    num=Convert_Decimal_number(s,l,h);
    for(i=dataLength-1;i>=0;i--)
    {
        temp[i]=num%2+'0';
        num = num/2;
    }
    temp[dataLength]='\0';

}

//This is utility function for fromAssemblyToMachine()

void from_assembly_to_machine(char *s,int l,int h,int *flag,int wordcount,FILE *out,int* x)
{
    char temp[50];
    int i;
    if(wordcount==1)
    {
        if(s[h-1]=='I')//If there is any immediate addressing mode then value of flag would be 1
        {
            (*flag)=1;
        }
        for(i=l;i<h;i++)
        {
            temp[i]=s[i];
        }
        temp[i]='\0';
        for(i=0;i<number_of_op_codes;i++)
        {
            if(!strcmp(Opcodes[i].text,temp))
            {
                fprintf(out,"%s",Opcodes[i].binary);
                return;
            }
        }
    }
    else
    {
        if(*flag==1&&wordcount==2)//constant value
        {
            if (s[l]=='#')
            {
                get_binary(s,l+1,h,temp);
                fprintf(out,"%s",temp);
                return;
            }// First Pass will 
            else
            {
                (*x)=2;
                return;
            }
        }
        else if(wordcount==2)//registers
        {
            for(i=l;i<h;i++)
            {
                temp[i-l]=s[i];
            }
            temp[i-l]='\0';
            for(i=0;i<number_of_op_registers;i++)
            {
                if(!strcmp(Register[i].text,temp))
                {
                    fprintf(out,"%s",Register[i].binary);
                    return;
                }
            }
            for(i=0;i<number_of_op_labels;i++)//if its not in register then its label
            {
                if(!strcmp(Label[i].text,temp))
                {
                    fprintf(out,"%s",Label[i].binary);
                    return;
                }
            }
        }
        else if(wordcount==3)//3rd part of instruction if exists is always register
        {
            for(i=l;i<h;i++)
            {
                temp[i-l]=s[i];
            }
            temp[i-l]='\0';
            for(i=0;i<number_of_op_registers;i++)
            {
                if(!strcmp(Register[i].text,temp))
                {
                    fprintf(out,"%s",Register[i].binary);
                    return;
                }
            }
        }
    }
}

//This function uses utilityMachineFunction and converts the machine code into assembly code

void SecondPass()
{
    FILE *as=fopen(filename,"r");
    FILE *out=fopen("Machine_File.txt","w");
    char s[50];
    int i,j,flag,wordcount,x;
    if(as==NULL)
    {
        printf("Unable to find the assembly code\n");
        return;
    }
    while(fgets(s,50,as)!=NULL)
    {
        s[strlen(s)-1]='\0';
        j=0;
        wordcount=0;
        flag=0;
        int z=0;
        int fl=0;
        for(i=0;i<=strlen(s);i++)
        {
            if(s[i]==' '||s[i]=='\0'||s[i]==',')
            {

                wordcount++;
                if(wordcount==1&&s[j]=='L')
                {
                    fl=1;
                    break;
                }
                from_assembly_to_machine(s,j,i,&flag,wordcount,out,&x);
                j=i+1;
                if(x==2)
                {
                    printf("Correct Syntax not found\n");
                    break;
                }
            }

        }
       if(!fl)
            fprintf(out,"\n");
       if(x==2)
            break;
    }
    fclose(as);
    fclose(out);
    if (x!=2)
        printf("Successfully Converted to Machine instruction\n");
}

//This function converts the given input in binary form
void Input_To_Machine(char* s,int l,int h,int wordcount,int* flag,int* x)
{
    char temp[50];
    int i;
    if (wordcount==1)
    {
        if(s[h-1]=='I')
        {
            (*flag)=1;
        }
        for(i=l;i<h;i++)
        {
            temp[i]=s[i];
        }
        temp[i]='\0';
        for(i=0;i<number_of_op_codes;i++)
        {
            if(!strcmp(Opcodes[i].text,temp))
            {
                printf("%s",Opcodes[i].binary);
                return;
            }
        }
    }
    else
    {
        if(*flag==1&&wordcount==2)
        {
            if (s[l]=='#')
            {
                get_binary(s,l+1,h,temp);
                printf("%s",temp);
                return;
            }
            else
            {
                (*x)=2;
                return;
            }
        }
        else if(wordcount==2)
        {
            for(i=l;i<h;i++)
            {
                temp[i-l]=s[i];
            }
            temp[i-l]='\0';
            for(i=0;i<number_of_op_registers;i++)
            {
                if(!strcmp(Register[i].text,temp))
                {
                    printf("%s",Register[i].binary);
                    return;
                }
            }
            for(i=0;i<number_of_op_labels;i++)
            {
                if(!strcmp(Label[i].text,temp))
                {
                    printf("%s",Label[i].binary);
                    return;
                }
            }
        }
        else if(wordcount==3)
        {
            for(i=l;i<h;i++)
            {
                temp[i-l]=s[i];
            }
            temp[i-l]='\0';
            for(i=0;i<number_of_op_registers;i++)
            {
                if(!strcmp(Register[i].text,temp))
                {
                    printf("%s",Register[i].binary);
                    return;
                }
            }
        }
    }
}
//function which separates different part of the mnemonic code given in input
void Separate_Mnemonic(char *temp)
{
    int hashcount=0;
    int floatcount=0;
    int commacount=0;
    int registercount=0;
    int u=0;
    temp[strlen(temp)-1]='\0';
    int w=0,i,j=0,flag,x,fl;
    while (temp[u]!='\0')
    {
    	if(temp[u]=='#')
    	{
    		hashcount++;
		}
		else if(temp[u]==',')
		{
			commacount++;	
		}
		else if(temp[u] == '.')
		{
			floatcount++;
		}
    	else if(temp[u]== 'R'  && temp[u+1] == 'R')
    	{
    		registercount++;
		}
		u++;
	}
    for(i=0;i<=strlen(temp);i++)
    {
    	if(hashcount > 1 || commacount >1 || floatcount > 1 || registercount>1 )
    	{
    		 printf("Correct Syntax not found\n");
                break;
		}
        if(temp[i]==' '||temp[i]=='\0'||temp[i]==',')
        {
            w++;
            if(w==1&&temp[j]=='L')
            {
                fl=1;
                break;
            }
            Input_To_Machine(temp,j,i,w,&flag,&x);
            j=i+1;
            if(x==2)
            {
                printf("Correct Syntax not found\n");
                break;
            }
        }

    }
    printf("\n");
}
//This is the main function which use cases to print the options available to us
int main()
{
    int option=0;

    FILE *fp=fopen("registers.txt","r");
    while(fscanf(fp,"%s %s",Register[number_of_op_registers].text,Register[number_of_op_registers].binary)!=EOF)
	{	
	number_of_op_registers++;
	}
    fclose(fp);

	FILE *file1=fopen("opcodes.txt","r");
    while(fscanf(file1,"%s %s",Opcodes[number_of_op_codes].text,Opcodes[number_of_op_codes].binary)!=EOF)
	{
	number_of_op_codes++;
	}
    fclose(file1);
	
	char temp[50];
    while(option!=8)
    {
    	printf("\n\n");
        printf("-----------Enter Input From Given Range ------------\n");
        printf("1.Print the register table\n");
        printf("2.Print the opcode table\n");
        printf("3.Convert assembly to binary\n");
        printf("4.Convert any input to binary\n");
        printf("5.Print the assembly code\n");
        printf("6.Print the machine code\n");
        printf("7.Print the label Table\n");
        printf("8.Exit\n");
		
		printf("Enter Your Option: ");
        scanf("%d",&option);
		printf("\n\n");
		switch (option)
		{
			case 1:
				getRegister();
				break;
				
			case 2:
				getOpcode();
				break;
				
			case 3:
				
				 printf("Enter your file name\n");
            scanf("%s",filename);
            int flag=firstpass();//this is pass 1 checks for errors in syntax and labels
            if (flag==0)
            {
                printf("Error in syntax\n");
                break;
            }
            Generate_Op_Labels();
            SecondPass();//pass 2  CONVERTS FROM ASSEMBLY TO MACHINE
            break;
            
            case 4:
            	
            getchar();
            printf("Enter the instruction for which you want a binary code to be generated: ");
            gets(temp);
            //printf("%s\n",t);
            printf("The output generated is: ");
            Separate_Mnemonic(temp);
            break;
            
            case 5:
            	Assemblycode();
            break;
				
         case 6:
         	Machinecode();
            break;
				
            case 7:
            	 Get_Label();
            break;
    
        case 8:
            	break;
            	
    
          	printf("Enter Correct Input\n");
	
    		break;
		}
    
    }
    return 0;
}
