/*
	��ѧ�����
*/
//==========================================================
//�Ƚϸߵ��ֽ�,���ı�ԭ����
//num1-num2
//>=0: jmp->cjmp_1
//<0 : jmp->cjmp_2
//tmp0
comparison_16b macro  num1_l,num1_h,num2_l,num2_h,cjmp_1,cjmp_2
   	mov  	a,num1_l	//�ȼ����ֽ�
   	sub    	a,num2_l	
   	mov  	a,num1_h	//�ټ����ֽ�
   	sbc    	a,num2_h	//
   	b0bts0 	FC         	//
   	jmp    	cjmp_1     	//�������0�� >=
   	jmp    	cjmp_2     	//�������1�� <
   	endm
//==================================================================================================
//��������16λ����
swap_16b macro num1_l,num1_h,num2_l,num2_h
	b0mov	a,num1_l
	b0xch	a,num2_l
	b0xch	a,num1_l
	b0mov	a,num1_h
	b0xch	a,num2_h
	b0xch	a,num1_h
	endm
//==================================================================================================
//num ++
cnt_inc_16b macro  num_l,num_h
   	incms	num1_l
   	jmp	$ + 3
   	incms	num_h
   	nop
   	endm
//==================================================================================================
// num1 = num1 + num2
add_16b macro num1_l,num1_h,num2_l,num2_h
   	b0mov	a,num2_l
   	add	num1_l,a
   	b0mov	a,num2_h
   	adc	num1_h,a
   	endm
//==================================================================================================
// num1 = num1 - num2
sub_16b macro num1_l,num1_h,num2_l,num2_h
   	b0mov	a,num1_l
   	sub	a,num2_l
	b0mov	num1_l,a
   	b0mov	a,num1_h
   	sbc	a,num2_h
	b0mov	num1_h,a
   	endm
//==================================================================================================
	__div_u8_u8:
	//8λ������
	//d_math_input0  = ������	
	//d_math_input1  = ����		
	//d_math_output0 = ��	
	//d_math_output1 = �м����(����)   	
		   clr		_d_math_output0    	//��0��
		   clr		_d_math_output1		//��0�м����(����)   
		   clr      _count
		div_u8_u8_0:
		   incms	_count	 			//count��һ
		   b0bclr	fc
		   rlcm		_d_math_output0    	//������һλ��
           	   //b0bclr	fc
		   rlcm		_d_math_input0     	//�������м��������1λ�����м����
		   rlcm		_d_math_output1
		   mov		a,_d_math_output1  	//�м������ȥ����
		   sub		a,_d_math_input1
		   b0bts0	fc 	 				//�ж��Ƿ��н�λ���н�λ���������ڳ�����û��λ������С�ڳ���
		   jmp		div_u8_u8_1
		   jmp		div_u8_u8_2
		div_u8_u8_1:         	 			//û�н�λ
		   mov		_d_math_output1,a   //�õ�����
		   b0bset	_d_math_output0.0
		   jmp		div_u8_u8_3
		div_u8_u8_2:		 					//����н�λ
		   //b0bclr	_d_math_output0.0
		div_u8_u8_3:
		   mov		a,#8	 			//�ж��Ƿ�ѭ����8��
		   cmprs	a,_count
		   jmp		div_u8_u8_0
		   ret

//==================================================================================================
/* 	__div_u16_u16:
		//16λ����16λ������
		//d_math_input0  = ������	
		//d_math_input1  = ����		
		//d_math_output0 = ��	
		//d_math_output1 = �м����(����)   	
		   clr		_d_math_output0    		//��0��
		   clr      	_d_math_output0 + 1
		   clr		_d_math_output1			//��0�м����(����)   
		   clr		_d_math_output1 + 1
		   clr      	_count
		div_u16_u16_0:
		   incms	_count	 				//count��һ
		   b0bclr	fc
		   rlcm		_d_math_output0    		//������1λ
		   rlcm     	_d_math_output0 + 1
		   //b0bclr	fc
		   rlcm		_d_math_input0     		//�������м��������1λ�����м����
		   rlcm     	_d_math_input0 + 1
		   rlcm		_d_math_output1
		   rlcm     	_d_math_output1 + 1

		   b0mov	a,_d_math_output1  		//�м������ȥ����
		   sub		a,_d_math_input1
		   b0mov    	_math_tmp0,a
		   b0mov    	a,_d_math_output1 + 1
		   sbc      	a,_d_math_input1 + 1
		   //b0mov    	_tmp1,a
		   b0bts0	fc 	 					//�ж��Ƿ��н�λ���н�λ���������ڳ�����û��λ������С�ڳ���
		   jmp		div_u16_u16_1
		   jmp		div_u16_u16_2
		div_u16_u16_1:         	 				//û�н�λ
		   b0mov	_d_math_output1 + 1,a   //�õ�����
		   b0mov    	a,_math_tmp0
		   b0mov    	_d_math_output1,a
		   b0bset	_d_math_output0.0
		   jmp		div_u16_u16_3
		div_u16_u16_2:		 					//����н�λ
		   //b0bclr	_d_math_output0.0
		div_u16_u16_3:
		   mov		a,#16	 				//�ж��Ƿ�ѭ����16��
		   cmprs	a,_count
		   jmp		div_u16_u16_0
		   ret*/
//==================================================================================================
 	__div_u24_u16:
		//24λ����16λ������
		//d_math_input0  = ������	
		//d_math_input1  = ����		
		//d_math_output0 = ��	
		//d_math_output1 = �м����(����)   	
		   clr          _d_math_input1 + 2		//�������λ
		   clr		_d_math_output0    		//��0��
		   clr      	_d_math_output0 + 1
		   clr      	_d_math_output0 + 2
		   clr		_d_math_output1			//��0�м����(����)   
		   clr		_d_math_output1 + 1		
		   clr		_d_math_output1 + 2
		   clr      	_count
		div_u24_u16_0:
		   incms	_count	 				//count��һ
		   b0bclr	fc
		   rlcm		_d_math_output0    		//������1λ
		   rlcm     	_d_math_output0 + 1
		   rlcm     	_d_math_output0 + 2	
		   //b0bclr	fc
		   rlcm		_d_math_input0     		//�������м��������1λ�����м����
		   rlcm     	_d_math_input0 + 1
		   rlcm     	_d_math_input0 + 2
		   rlcm		_d_math_output1
		   rlcm     	_d_math_output1 + 1
	    	   rlcm     	_d_math_output1 + 2

		   b0mov	a,_d_math_output1  		//�м������ȥ����
		   sub		a,_d_math_input1
		   b0mov    	_math_tmp0,a
		   b0mov    	a,_d_math_output1 + 1
		   sbc      	a,_d_math_input1 + 1
		   b0mov    	_math_tmp0 + 1,a
		   b0mov    	a,_d_math_output1 + 2
		   sbc      	a,_d_math_input1 + 2
		   //b0mov    	_tmp1,a
		   b0bts0	fc 	 					//�ж��Ƿ��н�λ���н�λ���������ڳ�����û��λ������С�ڳ���
		   jmp		div_u24_u16_1
		   jmp		div_u24_u16_2
		div_u24_u16_1:         	 				//û�н�λ
		   b0mov	_d_math_output1 + 2,a   //�õ�����
		   b0mov    	a,_math_tmp0
		   b0mov    	_d_math_output1,a
    		   b0mov    	a,_math_tmp0 + 1
		   b0mov    	_d_math_output1 + 1,a
		   b0bset	_d_math_output0.0
		   jmp		div_u24_u16_3
		div_u24_u16_2:		 					//����н�λ
		   //b0bclr	_d_math_output0.0
		div_u24_u16_3:
		   mov		a,#24	 				//�ж��Ƿ�ѭ����24��
		   cmprs	a,_count
		   jmp		div_u24_u16_0
		   ret

//==================================================================================================
	__mul_u16_u8: //2711Bû�г˷�ָ��
		//16λ�� x 8λ�� �˷�(���������24λ)
		//d_math_input0  = ������	
		//d_math_input1  = ����		
		//d_math_output0 = ��
		/*	b0mov	a,_d_math_input0+0
			mul	a,_d_math_input1+0
			b0mov	_d_math_output0,a
			b0mov	a,R
			b0mov	_d_math_output0+1,a
			b0mov	a,_d_math_input0+1
			mul	a,_d_math_input1+0
			add	_d_math_output0+1,a
			b0mov	a,R
			clr	_d_math_output0+2
			adc	_d_math_output0+2,a
			ret*/
			clr		_d_math_input0 + 2     	//�屻������λ
                        clr             _d_math_input1 + 2	//�������λ
			clr             _d_math_input1 + 1	//�������λ
		   	clr		_d_math_output0    	//��0��
			clr		_d_math_output0 + 1
			clr		_d_math_output0 + 2
		   	clr     	_count
		mul_u16_u8_0:
		   	incms		_count	 		//count��һ

			rrcm    	_d_math_input1      	//��������1λ
			b0bts1		fc  				
			jmp		mul_u16_u8_1

			b0mov   	a,_d_math_input0     	//�������ӵ���
			add     	_d_math_output0,a
			b0mov   	a,_d_math_input0 + 1
			adc     	_d_math_output0 + 1,a
			mov		a,_d_math_input0 + 2
			adc		_d_math_output0 + 2,a
		mul_u16_u8_1:
			b0bclr		fc         	 			
		   	rlcm    	_d_math_input0  	//����������
			rlcm    	_d_math_input0 + 1
			rlcm    	_d_math_input0 + 2
		mul_u16_u8_2:
		   	mov		a,#8	 		//�ж��Ƿ�ѭ����8��
		   	cmprs		a,_count
		   	jmp		mul_u16_u8_0
			ret
//==================================================================================================
/*		//16λ�� x 16λ�� �˷�(���������32λ)
		//d_math_input0  = ������	
		//d_math_input1  = ����		
		//d_math_output0 = ��
		//_math_tmp0
	__mul_u16_u16:
		call	__mul_u16_u8
		b0mov	a,_d_math_output0+0
		b0mov	_math_tmp0+0,a
		b0mov	a,_d_math_output0+1
		b0mov	_math_tmp0+1,a
		b0mov	a,_d_math_output0+2
		b0mov	_math_tmp0+2,a
		b0mov	a,_d_math_input1+1
		b0mov	_d_math_input1+0,a
		call	__mul_u16_u8
		b0mov	a,_d_math_output0+2
		b0mov	_d_math_output0+3,a
		b0mov	a,_d_math_output0+1
		b0mov	_d_math_output0+2,a
		b0mov	a,_d_math_output0+0
		b0mov	_d_math_output0+1,a
		b0mov	a,_math_tmp0+0
		b0mov	_d_math_output0+0,a 
		b0mov	a,_math_tmp0+1
		add	_d_math_output0+1,a
		b0mov	a,_math_tmp0+2
		adc	_d_math_output0+2,a
		mov	a,#(0)
		adc	_d_math_output0+3,a	
		ret*/
//==================================================================================================
//end files

