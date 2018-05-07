#include<stdint.h>
#include<stdio.h>
#include<string.h>
# define Nr 16
# define blocksize 16
uint8_t state[4][4];
static const uint8_t sbox[256] = {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static const uint8_t rsbox[256] = {
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };


  //key:
    //2b7e151628aed2a6abf7158809cf4f3c.
  // TweakKey:
    //3c8f262739bfe3b7bcf8269910e25f4dc3f4fc9088517fba6a2dea826151e7b2.
    
    uint8_t TK1[4][4]={{(uint8_t)0x2b,(uint8_t)0x28,(uint8_t)0xab,(uint8_t)0x09},
	                   {(uint8_t)0x7e,(uint8_t)0xae,(uint8_t)0xf7,(uint8_t)0xcf},
					   {(uint8_t)0x15,(uint8_t)0xd2,(uint8_t)0x15,(uint8_t)0x4f},
					   {(uint8_t)0x16,(uint8_t)0xa6,(uint8_t)0x88,(uint8_t)0x3c}};
					 
	uint8_t TK2[4][4]={{(uint8_t)0x3c,(uint8_t)0x39,(uint8_t)0xbc,(uint8_t)0x10},
					   {(uint8_t)0x8f,(uint8_t)0xbf,(uint8_t)0xf8,(uint8_t)0xe2},
					   {(uint8_t)0x26,(uint8_t)0xe3,(uint8_t)0x26,(uint8_t)0x5f},
					   {(uint8_t)0x27,(uint8_t)0xb7,(uint8_t)0x99,(uint8_t)0x4d}};
					   
	uint8_t TK3[4][4] = {{(uint8_t)0xc3,(uint8_t)0x88,(uint8_t)0x6a,(uint8_t)0x61},
					   	 {(uint8_t)0xf4,(uint8_t)0x51,(uint8_t)0x2d,(uint8_t)0x51},
					   	 {(uint8_t)0xfc,(uint8_t)0x7f,(uint8_t)0xea,(uint8_t)0xe7},
					   	 {(uint8_t)0x90,(uint8_t)0xba,(uint8_t)0x82,(uint8_t)0xb2}};
							
											   
	uint8_t RCON[17]= {(uint8_t)0x2f,(uint8_t)0x5e,(uint8_t)0xbc,(uint8_t)0x63,
	                 (uint8_t)0xc6,(uint8_t)0x97,(uint8_t)0x35,(uint8_t)0x6a,
					 (uint8_t)0xd4,(uint8_t)0xb3,(uint8_t)0x7d,(uint8_t)0xfa,
					 (uint8_t)0xef,(uint8_t)0xc5,(uint8_t)0x91,(uint8_t)0x39,
					 (uint8_t)0x72};
	
   
    
    uint8_t stk[17][4][4];
    
    void Subtweakeys()
    {
    	uint8_t h[4][4]={{1,5,9,13},{6,10,14,2},{11,15,3,7},{12,0,4,8}};
    	int r=0,i,j;
    	for( r=0;r<=Nr;r++)
    	{
    	    uint8_t rc[4][4] =
			        {{1,RCON[r],0,0},{2,RCON[r],0,0},{4,RCON[r],0,0},{8,RCON[r],0,0}};
    		for( j=0;j<4;j++)
    		{
    			for( i=0;i<4;i++)
    			{
    				stk[r][i][j] = ((TK1[i][j]^TK2[i][j])^TK3[i][j]);
    				stk[r][i][j] = (stk[r][i][j]^rc[i][j]);
				}
			}
		// calculate next TK1 and TK2 and TK3 using permutation function h
		// 0 1  2  3 4  5  6 7 8  9 10 11 12 13 14 15 
		// 1 6 11 12 5 10 15 0 9 14  3  4 13  2  7  8	
		
			for( j=0;j<4;j++)
			{
				for( i=0;i<4;i++)
				{
					rc[i][j] = TK1[h[i][j]%4][h[i][j]/4];
				}
			}
			for( j=0;j<4;j++)
			{
				for( i=0;i<4;i++)
				{
					TK1[i][j] = rc[i][j];
				}
			}
			for( j=0;j<4;j++)
			{
				for(i=0;i<4;i++)
				{
					uint8_t a = TK2[i][j];
					uint8_t b = a>>7;
					uint8_t c = a>>5;
					c = c%2;
					a = a<<1;
					a = a +(c^b); 
					TK2[i][j] = a;
				}
			}
			for( j=0;j<4;j++)
			{
				for( i=0;i<4;i++)
				{
					rc[i][j] = TK2[h[i][j]%4][h[i][j]/4];
				}
			}
			for( j=0;j<4;j++)
			{
				for( i=0;i<4;i++)
				{
					TK2[i][j] = rc[i][j];
				}
			}
			for( j=0;j<4;j++)
			{
				for(i=0;i<4;i++)
				{
					uint8_t a = TK3[i][j];
					uint8_t b = a%2;
					uint8_t c = a>>6;
					c = c%2;
					a = a>>1;
					c = (c^b); 
					c = c<<7;
					c = c+a; 
					TK3[i][j] = c;
				}
			}
			for( j=0;j<4;j++)
			{
				for( i=0;i<4;i++)
				{
					rc[i][j] = TK3[h[i][j]%4][h[i][j]/4];
				}
			}
			for( j=0;j<4;j++)
			{
				for( i=0;i<4;i++)
				{
					TK3[i][j] = rc[i][j];
				}
			}
		}
	}
	
static uint8_t getSBoxValue(uint8_t num)
{
  return sbox[num];
}

static uint8_t getSBoxInvert(uint8_t num)
{
  return rsbox[num];
} 
	void AddRoundKey(uint8_t round)
	{
		uint8_t i,j;
 		 for (i=0;i<4;++i)
  			{
    			for (j = 0; j < 4; ++j)
    			{
     			 state[i][j] ^= stk[round][i][j];
    			}
 			 }
	}
	
static void SubBytes(void)
{
  uint8_t i, j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      state[j][i] = getSBoxValue(state[j][i]);
    }
  }
}

static void ShiftRows(void)
{
  uint8_t temp;

  // Rotate first row 1 columns to left  
  temp           = state[1][0];
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

uint8_t M[4][4] = {{2, 3, 1, 1},{1, 2, 3, 1},{1, 1, 2, 3},{3, 1, 1, 2}};
uint8_t INVM[4][4] = {{14, 11, 13, 9},{9, 14, 11, 13},{13, 9, 14, 11},{11, 13, 9, 14}};

static void MixColumns(void)
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
	    	// multiplication is done in GF(2^8) with (x^8 + x^4 + x^3 + x + 1)
		    // as irreducible polynomial.
		 	uint8_t a,b,p;
			a = (uint8_t)M[i][k];
			b = (uint8_t)state[k][j];
			p = 0;
			while((a!=0)&&(b!=0))
			{
				uint8_t c = b%2;
				if(c)
				{
					p = p^a;
				}
	    		uint8_t carry = a>>7;
	    		a = (a<<1); 
	    		if(carry)
	    		{
	    			a = (a^((uint8_t)0x1b));
				}
				b = b>>1;
			}
		 	state_pr[i][j] ^= p;
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

static void InvMixColumns(void)
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
		   // multiplication is done in GF(2^8) with (x^8 + x^4 + x^3 + x + 1)
	       // as irreducible polynomial.
		 	uint8_t a,b,p;
			a = (uint8_t)INVM[i][k];
			b = (uint8_t)state[k][j];
			p = 0;
			while((a!=0)&&(b!=0))
			{
				uint8_t c = b%2;
				if(c)
				{
					p = p^a;
				}
	    		uint8_t carry = a>>7;
	    		a = (a<<1); 
	    		if(carry)
	    		{
	    			a = (a^((uint8_t)0x1b));
				}
				b = b>>1;
			}
		 	state_pr[i][j] ^= p;
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

static void InvSubBytes(void)
{
  uint8_t i,j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      state[j][i] = getSBoxInvert(state[j][i]);
    }
  }
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



 void Cipher(void)
{
  uint8_t round = 0;
   
  for (round = 0; round < Nr; ++round)
  {
  	AddRoundKey(round);
    SubBytes();
    ShiftRows();
    MixColumns();
  }
   AddRoundKey(Nr);
}

static void InvCipher(void)
{
  uint8_t round=0;
  
  for (round = Nr; round > 0; --round)
  {
  	AddRoundKey(round);
  	InvMixColumns();
    InvShiftRows();
    InvSubBytes();
    
  }
  AddRoundKey(0);
}

char    plain_text[1000000];
char   cipher_text[1000000];
uint8_t  plaintext[1000000];
uint8_t ciphertext[1000000];
int l;
void encrypt()
{
	Subtweakeys();
	int e=0,e1=0;
	while((e<l))
	{
		int i,j;
		for(j=0;j<4;j++)
		{
			for(i=0;i<4;i++)
			{
				state[i][j] = plaintext[e++];
			}
		}
		Cipher();
	 for(j=0;j<4;j++)
	 {
	 	for(i=0;i<4;i++)
	 	{
           ciphertext[e1++]=state[i][j];	 		
		 }
	 }
	}
}
void decrypt(void)
{
	int e=0,e1=0;
	while((e<l))
	{
		int i,j;
		for(j=0;j<4;j++)
		{
			for(i=0;i<4;i++)
			{
				state[i][j] = ciphertext[e++];
			}
		}
		InvCipher();
	 for(j=0;j<4;j++)
	 {
	 	for(i=0;i<4;i++)
	 	{
           plaintext[e1++]=state[i][j];	 		
		 }
	 }
	}
}
int main()
{
	int i,j;
	int r=0;
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
	printf("-------------- ciphertext in hexadecimal--------------\n ");
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
	printf("-------------- decrypted plaintext --------------\n");
	printf("%s\n",plain_text);
	return 0;
}
