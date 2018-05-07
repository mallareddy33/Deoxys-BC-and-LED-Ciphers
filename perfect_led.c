#include<stdint.h>
#include<stdio.h>
#include<string.h>
# define Nr 8
# define blocksize 8
uint8_t state[4][4];
static const uint8_t sbox[16] = {0x0c,0x05,0x06,0x0b,0x09,0x00,0x0a,0x0d,
                                 0x03,0x0e,0x0f,0x08,0x04,0x07,0x01,0x02};
                                 
                                 
static const uint8_t rsbox[16]= {0x05,0x0e,0x0f,0x08,0x0c,0x01,0x02,0x0d,
                                 0x0b,0x04,0x06,0x03,0x00,0x07,0x09,0x0a};                                

// key : 0123456789abcdef
uint8_t key[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,
            0x0e,0x0f};
uint8_t M[4][4] = {{4,1,2,2},{8,6,5,6},{0x0b,0x0e,0x0a,9},{2,2,0x0f,0x0b}};
uint8_t INVM[4][4] = {{0x0c,0x0c,0x0d,4},{3,8,4,5},{7,6,2,0x0e},{0x0d,9,9,0x0d}};

uint8_t stk[Nr+1][4][4];

char plain_text[1000000];
char cipher_text[1000000];
uint8_t plaintext[1000000];
uint8_t ciphertext[1000000];
int l = 0;
void Addroundkey(t)
{
	int i=0,j=0;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			state[i][j] ^=stk[t][i][j];
		}
	}
}
void subtweakeys()
{
	int r=0,i=0,j=0;
	for(r=0;r<=Nr;r++)
	{
		int e = 0;
		for( i=0;i<4;i++)
		{
			for( j=0;j<4;j++)
			{
				stk[r][i][j] = key[e++]; 
			}
		}
	}
}
uint8_t RC[8] = {0x01,0x03,0x07,0x0f,0x1f,0x3e,0x3d,0x3b};
uint8_t ks[8] = {0,1,0,0,0,0,0,0}; //ks = 64
void Addconstants(uint8_t t)
{
	int i=0,j=0;
	uint8_t rc = RC[t],rc0,rc1,rc2,rc3,rc4,rc5;
	uint8_t ks1 = 4,ks2 = 4,ks3 = 0,ks4 = 0;
	ks2 = 1^ks2;
	ks3 = 2^ks3;
	ks4 = 3^ks4;
	rc0 = rc%2;
	rc>>1;
	rc1 = rc%2;
	rc>>1;
	rc2 = rc%2;
	rc>>1;
	rc3 = rc%2;
	rc>>1;
	rc4 = rc%2;
	rc>>1;
	rc5 = rc%2;
	uint8_t a = rc5;
	a<<1;
	a = a+rc4;
	a<<1;
	a = a+rc3;
	uint8_t b = rc2;
	b<<1;
	b = b+rc1;
	b<<1;
	b = b+rc0;
	uint8_t ad[4][4] = {{ks1,a,0,0},{ks2,b,0,0},{ks3,a,0,0},{ks4,b,0,0}};
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			state[i][j]^=ad[i][j];
		}
	}
}
void Subcells()
{
	int i,j = 0;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			state[i][j] = sbox[state[i][j]];
		}
	}
}
void InvSubcells()
{
		int i,j = 0;
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				state[i][j] = rsbox[state[i][j]];
			}
		}
}
static void ShiftRows(void)
{
  uint8_t temp;

  // Rotate first row 1 columns to left  
  temp        = state[1][0];
  state[1][0] = state[1][1];
  state[1][1] = state[1][2];
  state[1][2] = state[1][3];
  state[1][3] = temp;

  // Rotate second row 2 columns to left  
  temp           = state[2][0];
  state[2][0] = state[2][2];
  state[2][2] = temp;

  temp           = state[2][1];
  state[2][1] = state[2][3];
  state[2][3] = temp;

  // Rotate third row 3 columns to left
  temp           = state[3][0];
  state[3][0] = state[3][3];
  state[3][3] = state[3][2];
  state[3][2] = state[3][1];
  state[3][1] = temp;
}

static void InvShiftRows(void)
{
  uint8_t temp;

  // Rotate first row 1 columns to right  
  temp = state[1][3];
  state[1][3] = state[1][2];
  state[1][2] = state[1][1];
  state[1][1] = state[1][0];
  state[1][0] = temp;

  // Rotate second row 2 columns to right 
  temp = state[2][0];
  state[2][0] = state[2][2];
  state[2][2] = temp;

  temp = state[2][1];
  state[2][1] = state[2][3];
  state[2][3] = temp;

  // Rotate third row 3 columns to right
  temp = state[3][0];
  state[3][0] = state[3][1];
  state[3][1] = state[3][2];
  state[3][2] = state[3][3];
  state[3][3] = temp;
}


void MixColumnSerial()
{
	uint8_t state_pr[4][4]; 
	int i=0,j=0,k=0;
  for(i=0;i<4;i++)
  {
  	for(j=0;j<4;j++)
  	{
  		state_pr[i][j]=0;
  	   for(k=0;k<4;k++)
		 {
	           uint8_t a,b,p;
		         a = M[i][k];
			     b = state[k][j];
				 p = 0;
				while((a!=0)&&(b!=0))
				{
					uint8_t c = b%2;
					if(c)
					{
						p = p^a;
					}
	    			uint8_t carry = a>>3;
	   				carry = carry%2;
	    			a = (a<<1); 
	    			if(carry)
	    			{
	    				a = a^(1<<4);
	    				a = (a^((uint8_t)0x03));
					}
					b = b>>1;
				}
				state_pr[i][j]^=p;		
		 }
	  }
  }
  for(i=0;i<4;i++)
  {
  	for(j=0;j<4;j++)
  	{
  	   state[i][j] = state_pr[i][j];
	}
  }
}
void InvMixColumnSerial()
{
		uint8_t state_pr[4][4]; 
	int i=0,j=0,k=0;
  for(i=0;i<4;i++)
  {
  	for(j=0;j<4;j++)
  	{
  		state_pr[i][j]=0;
  	   for(k=0;k<4;k++)
		 {
	           uint8_t a,b,p;
		         a = INVM[i][k];
			     b = state[k][j];
				 p = 0;
				while((a!=0)&&(b!=0))
				{
					uint8_t c = b%2;
					if(c)
					{
						p = p^a;
					}
	    			uint8_t carry = a>>3;
	   				carry = carry%2;
	    			a = (a<<1); 
	    			if(carry)
	    			{
	    				a = a^(1<<4);
	    				a = (a^((uint8_t)0x03));
					}
					b = b>>1;
				}
				state_pr[i][j]^=p;		
		 }
	  }
  }
  for(i=0;i<4;i++)
  {
  	for(j=0;j<4;j++)
  	{
  	   state[i][j] = state_pr[i][j];
	}
  }
}
void step(uint8_t t)
{
	Addconstants(t);
	Subcells();
	ShiftRows();
	MixColumnSerial();
}
void Invstep(uint8_t t)
{
	InvMixColumnSerial();
	InvShiftRows();
	InvSubcells();
	Addconstants(t);

	
	
}
void cipher()
{
	uint8_t t =0;
	for(t=0;t<Nr;t++)
	{
		Addroundkey(t);
		step(t);		
	}
    Addroundkey(Nr);
}
void Invcipher()
{
	uint8_t t;
	for(t = Nr;t>0;t--)
	{
		Addroundkey(t);
		Invstep(t-1);
	}
	Addroundkey(0);
}
static void decrypt()
{
	int e = 0,f =0,e1=0,i=0,j=0;
	while(e<l)
	{
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(f==0)
				{
					state[i][j] = (ciphertext[e]>>4);
					f=1;
				}
				else
				{
					state[i][j] = ciphertext[e]%16;
					f=0;
					e++;
				}
			}
		}
		Invcipher();
		f=0;
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(f==0)
				{
					plaintext[e1]=state[i][j];
					f=1;
				}
				else
				{
					plaintext[e1]=plaintext[e1]<<4;
					plaintext[e1]=plaintext[e1]+state[i][j];
					e1++;
					f=0;
				}
			}
		}
 	}
}
static void encrypt()
{
	subtweakeys();
	int e = 0,f =0,e1=0,i=0,j=0;
	while(e<l)
	{
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(f==0)
				{
					state[i][j] = (plaintext[e]>>4);
					f=1;
				}
				else
				{
					state[i][j] = plaintext[e]%16;
					f=0;
					e++;
				}
			}
		}
		cipher();
		f=0;
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(f==0)
				{
					ciphertext[e1]=state[i][j];
					f=1;
				}
				else
				{
					ciphertext[e1]=ciphertext[e1]<<4;
					ciphertext[e1]=ciphertext[e1]+state[i][j];
					e1++;
					f=0;
				}
			}
		}
 	}
}


int main()
{

	int i,j;
	printf("enter plain text\n");
	scanf("%s",plain_text);
	l = strlen(plain_text);
	while((l%blocksize)!=0)
	{
		plain_text[l++]='0';
	}
	plain_text[l] = '\0';
	for( i=0;i<l;i++)
	{
		plaintext[i] = (uint8_t)(plain_text[i]);
	}
	encrypt();
	printf("\n-------------- ciphertext in hexadecimal--------------\n ");
	for( i=0;i<l;i++)
	{
		cipher_text[i] = (char)(ciphertext[i]);
		printf("%02x",ciphertext[i]);
	}
	printf("\n--------- the ciphertext in characters is  -------\n");
	printf("%s\n",cipher_text);
	decrypt();
	for( i=0;i<l;i++)
	{
		plain_text[i] = (char)(plaintext[i]);
	}
	printf("\n-------------- decrypted plaintext --------------\n");
	printf("%s\n",plain_text);
	return 0;
}
