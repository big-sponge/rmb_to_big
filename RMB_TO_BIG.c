/****************************************************************************
 *
 * 函数名称: 
 * 功能描述: 将用阿拉伯数字表示的金额转换为用中文大写数字表示。
 *           金额范围：-9999999999999999.99 —— 9999999999999999.99 
             数量级：  千万亿
             精确值：  小数点后两位 
 * 输入参数: 阿拉伯数字表示的金额
 *                      
 * 输出参数: 中文大写数字表示的金额
 * 返 回 值: 
 *           
 *
 ***************************************************************************
 * Who            Date       Action       Memo
 * ChengCheng			2011-07-20		cc@tkc8.com
 ***************************************************************************
 * TIP: C-Create M-Modify
 ***************************************************************************/
 
/*---------------------------- 头文件定义 ------------------------*/
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<unistd.h>
/*--------------------------- 头文件定义结束 -------------------------*/

/*------------------------ 以下内容相关函数定义 ------------------------*/

#define	MAX_SMALL_AMOUNT_LEN            19
#define	SMALL_AMOUNT_LEN                MAX_SMALL_AMOUNT_LEN
#define SMALL_PART_LEN                  2
#define	BIG_AMOUNT_LEN                  128
#define	ONE_WORD_LEN                    2
/*------------------------ 以上内容相关函数定义 ------------------------*/


/*
 *去空格
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
 * 检查金额的有效性
 */
int amount_is_valid( char *buffer, int len )
{
   long ll_loop = 0;
   int	li_dots = 0;

   /* --判断输入参数的有效性，如果输入参数不合法，则视为金额无效-- */
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
 *转换成大写
 */  
char * amount_switch_small_to_big( char *szNum )
{
     
   /* --定义-- */
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
   
   /* --初始化-- */
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

   /* --检查输入参数的有效性-- */
   if( !szNum || !strlen( szNum ))
   {
      return( "ERROR" );
   }
   
   /* --去空格-- */
   amount_trim( szNum, 'A' );
   
   /* --检查金额的有效性-- */
   if( !amount_is_valid( szNum, strlen( szNum )))
   {
      return( "Invalid Amount" );	
   }
   
   /* --判断是否为负数-- */
   if( szNum[ 0 ] == '-') 
   {
      szNum++; 
      strcpy( CapsBuf, "负"	);
   }
   
   /* -- 格式化 -- */ 
   /* ---- 小数部分格式化 ---- */
   /* ------ 找到小数点的位置，如果没有小数点point_pos的值为strlen( buf ) ------ */    
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
   /*sprintf( stderr, "小数部分格式化后：%s", ( char * )szNum );*/
   
   /* ---- 整数部分格式化 ----*/    
   /* ------ 去掉小写金额前面可能存在的多余的零 ------ */
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
   /* -- 格式换完毕 --*/
                    
   /* -- 输入长度验证 -- */
   /*
   if( point_pos + SMALL_PART_LEN + 1 < strlen( szNum ) ) 
   {  	
      sprintf( CapsBuf, "小数部分长度超过：%d", SMALL_PART_LEN );
      return( CapsBuf );     
   } 
   */
   if( point_pos + SMALL_PART_LEN + 1 > MAX_SMALL_AMOUNT_LEN ) 
   {  	
      sprintf( CapsBuf, "整数部分长度超过：%d", MAX_SMALL_AMOUNT_LEN - SMALL_PART_LEN - 1 );
      return( CapsBuf );
   } 
   
   if( szNum[0]=='.' )
   {
      buf[0] = '0';	   	
   }            
   strcat( buf, szNum );
          
   /************ 执行转换 *******************/ 
   /* -- 循环处理 -- */
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
      /* 数值位转换阶段 */
      switch( buf[ index ] )
      {
         case '0':
            sprintf( big, "零" ); 
            f_power = 1;
            f_big = 1; 
            
            /*  解决什么时候显示 "零" */
            if(( index + 1 ) < strlen( buf ))
            {
               if( buf[ index + 1 ] != '0' )
               {
                  f_big = 0;
               }     	  	
            }      	           	                      
            break;
         case '1':
            sprintf( big, "壹" );
            break;
         case '2':
            sprintf( big, "贰" );
            break;
         case '3':
            sprintf( big, "叁" );
            break;
         case '4':
            sprintf( big, "肆" );
            break;
         case '5':
            sprintf( big, "伍" );
            break;
         case '6':
            sprintf( big, "陆" );
            break;
         case '7':
            sprintf( big, "柒" );
            break;
         case '8':
            sprintf( big, "捌" );
            break;
         case '9':
            sprintf( big, "玖" );
            break;
         default:
            f_power = 1;
            f_big = 1; 
            break;
      }        	
            
      /* 权位转换阶段 */          
      switch( point_pos - index )
      {         										         
         case 13:
         case 5:
            sprintf( power, "万" );        	      
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
            sprintf( power, "仟" );
            break;
         case 15:
         case 11:
         case 7:
         case 3:
            sprintf( power, "佰" );
            break;
         case 14:
         case 10:
         case 6:
         case 2:
            sprintf( power, "拾" );
            break;
         case 9:
            sprintf( power, "亿" );
            f_power = 0;
            if( buf[ index ] == '0' )
            {
               f_big = 1;
            }
            break;
         case 1:
            sprintf( power, "元" );
            if( buf[ index ] == '0' )
            {
            	f_big = 1;
            }
            
            /* 解决什么时候不显示"元" */
            if( position == 0 && f_big == 1 && !is_small_zero )
            {
               f_power = 1;
            }
            else
            {
               f_power = 0;
            }
            
            /* 解决什么时候显示"0" */
            if( position == 0 && f_big == 1 && is_small_zero )
            {
               f_big = 0;
            }                     	         	       	  
            break;
         case 0:
            sprintf( power, "整" );
            f_power = 1;
            if( is_small_zero )
            {
               f_power = 0;
            }  
            break;
         case -1:
            sprintf( power, "角" );
            /* 解决什么时候不显示"0" */
            if( position == 0 && buf[ index ] == '0')
            {
               f_big = 1;
            }
            break;
         case -2:
            sprintf( power, "分" );
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
   /* 循环处理完毕 */

   str[ position ] = 0x00;
   strcat( CapsBuf, str );
   return( CapsBuf );
}

int main()
{
   char ch[ 50 ];
   fprintf( stderr, "请输入小写金额: " );
   fflush( stdin );
   gets( ch );
   fflush( stdin );
   fprintf( stderr, "转换后大写金额: [%s]\n", ( char * )amount_switch_small_to_big( ch ));
   return( 0 );
}