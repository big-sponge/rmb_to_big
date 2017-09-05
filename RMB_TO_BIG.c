/****************************************************************************
 *
 * ��������: 
 * ��������: ���ð��������ֱ�ʾ�Ľ��ת��Ϊ�����Ĵ�д���ֱ�ʾ��
 *           ��Χ��-9999999999999999.99 ���� 9999999999999999.99 
             ��������  ǧ����
             ��ȷֵ��  С�������λ 
 * �������: ���������ֱ�ʾ�Ľ��
 *                      
 * �������: ���Ĵ�д���ֱ�ʾ�Ľ��
 * �� �� ֵ: 
 *           
 *
 ***************************************************************************
 * Who            Date       Action       Memo
 * ChengCheng			2011-07-20		cc@tkc8.com
 ***************************************************************************
 * TIP: C-Create M-Modify
 ***************************************************************************/
 
/*---------------------------- ͷ�ļ����� ------------------------*/
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<unistd.h>
/*--------------------------- ͷ�ļ�������� -------------------------*/

/*------------------------ ����������غ������� ------------------------*/

#define	MAX_SMALL_AMOUNT_LEN            19
#define	SMALL_AMOUNT_LEN                MAX_SMALL_AMOUNT_LEN
#define SMALL_PART_LEN                  2
#define	BIG_AMOUNT_LEN                  128
#define	ONE_WORD_LEN                    2
/*------------------------ ����������غ������� ------------------------*/


/*
 *ȥ�ո�
 */ 
void amount_trim ( char *s, char Mode )
{
   int  i ;
   char *p ;

   if ( s == NULL ) 
   {
      return ;
   }
   if ( strlen ( s ) == 0 )  
   {
   	  return ;
   }

   p = s ;

   switch ( toupper ( Mode ) )
   {
      case 'A':

      case 'L':
         while ( *p==' ' || *p== '\t' ) p++ ;
         strcpy ( s , p ) ;
         if(toupper ( Mode )== 'L' ){break ;}

      case 'R':
         while ( ( strlen ( s ) >= 1 ) &&
                 ( ( s [ strlen ( s ) - 1 ] == ' ' ) ||
                   ( s [ strlen ( s ) - 1 ] == '\t' ) ) )
            s [ strlen ( s ) - 1 ] = 0x00 ;
         break ;      
   }
   return ;
} 

/*
 * ��������Ч��
 */
int amount_is_valid( char *buffer, int len )
{
   long ll_loop = 0;
   int	li_dots = 0;

   /* --�ж������������Ч�ԣ��������������Ϸ�������Ϊ�����Ч-- */
   if( buffer == NULL || len <= 0 )
   {
      return( 0 );
   }
   else
   {
      for( ll_loop = 0; ll_loop < len; ll_loop++ )
      {
         if( ll_loop == 0 && buffer[ ll_loop ] == '-' )
         {
            continue;
         }

         if( buffer[ ll_loop ] == '.' )
         {
            li_dots++;
         }

         if(( buffer[ ll_loop ] < '0' || buffer[ ll_loop ] > '9' ) && buffer[ ll_loop ] != '.' )
         {
            return( 0 );
         }
      }
   }

   if( li_dots >= 2 )
   {
      return( 0 );
   }

   return( 1 );
}

/*
 *ת���ɴ�д
 */  
char * amount_switch_small_to_big( char *szNum )
{
     
   /* --����-- */
   static char CapsBuf[ 256 ];
   char buf[ SMALL_AMOUNT_LEN + 1 ];
   char str[ BIG_AMOUNT_LEN + 1 ];
   char power[ ONE_WORD_LEN + 1 ];
   char big[ ONE_WORD_LEN + 1 ];
   int  position;
   int  point_pos;
   int  index;
   int  f_big;
   int  f_power;
   int  is_small_zero;
   int  i;
   
   /* --��ʼ��-- */
   memset(( char * )CapsBuf, 0x00, sizeof( CapsBuf ));
   index = 0;
   position = 0;
   f_big = 0;
   f_power = 0;
   is_small_zero = 0;
   i = 0;
   point_pos = MAX_SMALL_AMOUNT_LEN - SMALL_PART_LEN - 1;
   memset(( char * )CapsBuf, 0x00, sizeof( CapsBuf ));
   memset(( char * )buf, 0x00, sizeof( buf ));
   memset(( char * )str, 0x00, sizeof( str ));
   memset(( char * )power, 0x00, sizeof( power ));
   memset(( char * )big, 0x00, sizeof( big ));

   /* --��������������Ч��-- */
   if( !szNum || !strlen( szNum ))
   {
      return( "ERROR" );
   }
   
   /* --ȥ�ո�-- */
   amount_trim( szNum, 'A' );
   
   /* --��������Ч��-- */
   if( !amount_is_valid( szNum, strlen( szNum )))
   {
      return( "Invalid Amount" );	
   }
   
   /* --�ж��Ƿ�Ϊ����-- */
   if( szNum[ 0 ] == '-') 
   {
      szNum++; 
      strcpy( CapsBuf, "��"	);
   }
   
   /* -- ��ʽ�� -- */ 
   /* ---- С�����ָ�ʽ�� ---- */
   /* ------ �ҵ�С�����λ�ã����û��С����point_pos��ֵΪstrlen( buf ) ------ */    
   point_pos = 0;
   while( szNum[ point_pos ] != '.' && ( point_pos < strlen( szNum ) ) ) 
   {
      point_pos++;   	
   }
   if( point_pos == strlen( szNum ))
   {
      strcat( szNum, "." );
   }
   while( strlen( szNum ) - point_pos < SMALL_PART_LEN + 1 )
   {
   	  strcat( szNum, "0" ); 
   } 
   szNum[ point_pos + SMALL_PART_LEN + 1 ] = 0x00;
   /*sprintf( stderr, "С�����ָ�ʽ����%s", ( char * )szNum );*/
   
   /* ---- �������ָ�ʽ�� ----*/    
   /* ------ ȥ��Сд���ǰ����ܴ��ڵĶ������ ------ */
   i = 0;   
   while( szNum[ i ] == '0')
   {
      if( i < strlen( szNum ) && szNum[ i + 1 ] != '.')
      {
         szNum++;
      }else
      {
         i++;
      }	
   }   
   /* -- ��ʽ����� --*/
                    
   /* -- ���볤����֤ -- */
   /*
   if( point_pos + SMALL_PART_LEN + 1 < strlen( szNum ) ) 
   {  	
      sprintf( CapsBuf, "С�����ֳ��ȳ�����%d", SMALL_PART_LEN );
      return( CapsBuf );     
   } 
   */
   if( point_pos + SMALL_PART_LEN + 1 > MAX_SMALL_AMOUNT_LEN ) 
   {  	
      sprintf( CapsBuf, "�������ֳ��ȳ�����%d", MAX_SMALL_AMOUNT_LEN - SMALL_PART_LEN - 1 );
      return( CapsBuf );
   } 
   
   if( szNum[0]=='.' )
   {
      buf[0] = '0';	   	
   }            
   strcat( buf, szNum );
          
   /************ ִ��ת�� *******************/ 
   /* -- ѭ������ -- */
   point_pos = strlen( buf ) - SMALL_PART_LEN - 1;   
   i = 1;
   while( buf[ point_pos + i ] == '0' )
   {
      i++;
   }
   if( i > SMALL_PART_LEN  )
   {
      is_small_zero = 1;
   }
   
   position = 0;            
   index = 0;   
   while( index < strlen( buf ) )
   {
      /* ��ֵλת���׶� */
      switch( buf[ index ] )
      {
         case '0':
            sprintf( big, "��" ); 
            f_power = 1;
            f_big = 1; 
            
            /*  ���ʲôʱ����ʾ "��" */
            if(( index + 1 ) < strlen( buf ))
            {
               if( buf[ index + 1 ] != '0' )
               {
                  f_big = 0;
               }     	  	
            }      	           	                      
            break;
         case '1':
            sprintf( big, "Ҽ" );
            break;
         case '2':
            sprintf( big, "��" );
            break;
         case '3':
            sprintf( big, "��" );
            break;
         case '4':
            sprintf( big, "��" );
            break;
         case '5':
            sprintf( big, "��" );
            break;
         case '6':
            sprintf( big, "½" );
            break;
         case '7':
            sprintf( big, "��" );
            break;
         case '8':
            sprintf( big, "��" );
            break;
         case '9':
            sprintf( big, "��" );
            break;
         default:
            f_power = 1;
            f_big = 1; 
            break;
      }        	
            
      /* Ȩλת���׶� */          
      switch( point_pos - index )
      {         										         
         case 13:
         case 5:
            sprintf( power, "��" );        	      
            f_power = 0;
            if( buf[ index ] == '0' )
            {
            	f_big = 1;
            }
            if( point_pos > 8 && point_pos - index == 5)
            {
               if( buf[ point_pos - 5 ] == '0' && buf[ point_pos - 6 ] == '0' 
                         && buf[ point_pos - 7 ] == '0' && buf[ point_pos - 8 ] == '0')
               {
                  f_power = 1;
               }
            }
            break;   
         case 16:
         case 12:
         case 8:
         case 4:
            sprintf( power, "Ǫ" );
            break;
         case 15:
         case 11:
         case 7:
         case 3:
            sprintf( power, "��" );
            break;
         case 14:
         case 10:
         case 6:
         case 2:
            sprintf( power, "ʰ" );
            break;
         case 9:
            sprintf( power, "��" );
            f_power = 0;
            if( buf[ index ] == '0' )
            {
               f_big = 1;
            }
            break;
         case 1:
            sprintf( power, "Ԫ" );
            if( buf[ index ] == '0' )
            {
            	f_big = 1;
            }
            
            /* ���ʲôʱ����ʾ"Ԫ" */
            if( position == 0 && f_big == 1 && !is_small_zero )
            {
               f_power = 1;
            }
            else
            {
               f_power = 0;
            }
            
            /* ���ʲôʱ����ʾ"0" */
            if( position == 0 && f_big == 1 && is_small_zero )
            {
               f_big = 0;
            }                     	         	       	  
            break;
         case 0:
            sprintf( power, "��" );
            f_power = 1;
            if( is_small_zero )
            {
               f_power = 0;
            }  
            break;
         case -1:
            sprintf( power, "��" );
            /* ���ʲôʱ����ʾ"0" */
            if( position == 0 && buf[ index ] == '0')
            {
               f_big = 1;
            }
            break;
         case -2:
            sprintf( power, "��" );
            break;
      	 default:
            f_power = 1;
            break;
      } 

      if(!f_big)
      {
         str[ position++ ] = big[ 0 ];
         str[ position++ ] = big[ 1 ];
      }
      if(!f_power)
      {          
         str[ position++ ] = power[ 0 ];
         str[ position++ ] = power[ 1 ];     
      }
      f_big = 0;
      f_power = 0; 
      index++;                    
   } 
   /* ѭ��������� */

   str[ position ] = 0x00;
   strcat( CapsBuf, str );
   return( CapsBuf );
}

int main()
{
   char ch[ 50 ];
   fprintf( stderr, "������Сд���: " );
   fflush( stdin );
   gets( ch );
   fflush( stdin );
   fprintf( stderr, "ת�����д���: [%s]\n", ( char * )amount_switch_small_to_big( ch ));
   return( 0 );
}