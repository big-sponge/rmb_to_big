###  这个是很早之前用C语言实现的人民币小写转大写的方法


编辑后直接运行就可以，Linux、Windows都可以

```

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

```
 