struct Azrael64 {
	
}

impl Azrael64 {

	fn evalua_func_bool( &self, x1: i64, x2: i64, x3: i64, x4: i64, x5: i64) -> i64 {
		
		let c_0: i64 = (( x1.wrapping_add(x2) ) ^ ( x3 ^ x4 )) ^ x5;
		let c_1: i64 = (( x1 & x2 ) ^ ( x3.wrapping_add(x4) )) ^ x5;
		let c_2: i64 = ( (x1 ^ x2).wrapping_add( x3.wrapping_add(x4)) ) ^ x5;
		let c_3: i64 = (( x1 ^ x2 ) ^ ( x3.wrapping_add(x4) )) ^ x5;
		let c_4: i64 = ( (x1 & x2).wrapping_add( x3.wrapping_add(x4)) ).wrapping_add(x5);
		let c_5: i64 = ( (x1 & x2).wrapping_add( x3.wrapping_add(x4)) ) ^ x5;
		let c_6: i64 = (( x1 ^ x2 ) ^ ( x3.wrapping_add(x4) )) ^ x5;
		let c_7: i64 = (( x1 | x2 ) ^ ( x3.wrapping_add(x4) )) ^ x5;
		let c_8: i64 = (  x1 | x2 ) | ((x3.wrapping_add(x4) )  ^ x5);
		let c_9: i64 = ( (x1.wrapping_add(x2)).wrapping_add( x3.wrapping_add(x4)) ) ^ x5;
		let c_a: i64 = ( (x1.wrapping_add(x2)) & ( x3.wrapping_add(x4) )) ^ x5;
		let c_b: i64 = ( (x1 ^ x2).wrapping_add( x3 ^ x4 ) ) ^ x5;
		let c_c: i64 = (( x1 | x2 ) ^ ( x3 ^ x4 )) ^ x5;
		let c_d: i64 = ( (x1.wrapping_add(x2)).wrapping_add( x3 ^ x4 ) ) ^ x5;
		let c_e: i64 = ( (x1.wrapping_add(x2)) ^ x3.wrapping_add(x4) ) ^ x5;
		let c_f: i64 = x1.wrapping_add(x2) & x3.wrapping_add(x4).wrapping_add(x5);

		return c_0.wrapping_add(c_1).wrapping_add(c_2).wrapping_add(c_3)
			.wrapping_add(c_4).wrapping_add(c_5).wrapping_add(c_6).wrapping_add(c_7)
			.wrapping_add(c_8).wrapping_add(c_9).wrapping_add(c_a).wrapping_add(c_b)
			.wrapping_add(c_c).wrapping_add(c_d).wrapping_add(c_e).wrapping_add(c_f);
	}
	
	fn pad( &self, data: &[u8], _output: &mut Vec<i8>, length: usize, padding: usize ) {
		let mut pad: Vec<i8> = vec![0;padding];
        pad[0] = -0x80 as i8;
        let bits = length * 8;
        for i in 0..8 {
			pad[padding - 1 - i] = ((bits >> (8 * i)) & 0xFF) as i8;
        }
		for i in 0..length {
			_output[i] = data[i] as i8;
		}
        for i in 0..padding {
			_output[length+i] = pad[i] as i8;
		}
	}
	
	pub fn get_hash_eval( &self, data: &[u8], input: &mut Vec<i8>, length1: usize, padding: usize, hash1: &mut i64 ) {
		self.pad( data,input,length1,padding );

		let length = input.len();
		
		let iv1:i64 = 0x6a09e667bb67ae85;
		let iv2:i64 = 0x3c6ef372a54ff53a;
		let iv3:i64 = 0x510e527f9b05688c;
		let iv4:i64 = 0x1f83d9ab5be0cd19;
		let iv5:i64 = 0x428a2f9871374491;
		let iv6:i64 = -0x4A3F0430164A245B; //0xb5c0fbcfe9b5dba5;
		let iv7:i64 = 0x3956c25b59f111f1;
		let iv8:i64 = -0x6DC07D5B54E3A12B; //0x923f82a4ab1c5ed5L;
		let iv9:i64 = -0x27F85567ED7CA4FF; //0xd807aa9812835b01L;
		let iv10:i64 = 0x243185be550c7dc3;

		let mut char1:i64=iv1;
		let mut char2:i64=iv2;
		let mut char3:i64=iv3;
		let mut char4:i64=iv4;
		let mut char5:i64=iv5;
		
		let mut suma_ant1:i64 = iv6;
		let mut suma_ant2:i64 = iv7;
		let mut suma_ant3:i64 = iv8;
		let mut suma_ant4:i64 = iv9;
		let mut suma_ant5:i64 = iv10;
		
		char1 = char1.wrapping_add( input[ length-2 ] as i64 );
		char2 = char2.wrapping_add( input[ length-1 ] as i64 );
		char3 = char3.wrapping_add( input[ 0 ] as i64 );
		char4 = char4.wrapping_add( input[ 1 ] as i64 );
		char5 = char5.wrapping_add( input[ 2 ] as i64 );
		suma_ant5 = suma_ant5.wrapping_add(suma_ant4);
		suma_ant4 = suma_ant4.wrapping_add(suma_ant3);
		suma_ant3 = suma_ant3.wrapping_add(suma_ant2);
		suma_ant2 = suma_ant2.wrapping_add(suma_ant1);
		suma_ant1 = suma_ant1.wrapping_add(self.evalua_func_bool( char1,char2,char3,char4,char5));

		// Main Loop.
		for i in 1..length-1 { 
			char1 = char1.wrapping_add(suma_ant1);
			char2 = char2.wrapping_add(char3);
			char3 = char3.wrapping_add(char4);
			char4 = char4.wrapping_add(input[ i+1 ] as i64);
			char5 = char5.wrapping_add(suma_ant2);
			suma_ant5 = suma_ant5.wrapping_add(suma_ant4);
			suma_ant4 = suma_ant4.wrapping_add(suma_ant3);
			suma_ant3 = suma_ant3.wrapping_add(suma_ant2);
			suma_ant2 = suma_ant2.wrapping_add(suma_ant1);
			suma_ant1 = suma_ant1.wrapping_add(self.evalua_func_bool( char1,char2,char3,char4,char5));
		}
		
		char1 = char1.wrapping_add(suma_ant1);
		char2 = char2.wrapping_add(char3);
		char3 = char3.wrapping_add(char4);
		char4 = char4.wrapping_add(input[ 0 ] as i64);
		char5 = char5.wrapping_add(suma_ant2);
		suma_ant5 = suma_ant5.wrapping_add(suma_ant4);
		suma_ant4 = suma_ant4.wrapping_add(suma_ant3);
		suma_ant3 = suma_ant3.wrapping_add(suma_ant2);
		suma_ant2 = suma_ant2.wrapping_add(suma_ant1);
		suma_ant1 = suma_ant1.wrapping_add(self.evalua_func_bool( char1,char2,char3,char4,char5));

		suma_ant1 = suma_ant1.wrapping_add(self.evalua_func_bool( suma_ant1,suma_ant1,suma_ant1,suma_ant1,suma_ant1)).wrapping_add(iv1);
		suma_ant2 = suma_ant2.wrapping_add(self.evalua_func_bool( suma_ant2,suma_ant2,suma_ant2,suma_ant2,suma_ant2)).wrapping_add(iv2);
		suma_ant3 = suma_ant3.wrapping_add(self.evalua_func_bool( suma_ant3,suma_ant3,suma_ant3,suma_ant3,suma_ant3)).wrapping_add(iv3);
		suma_ant4 = suma_ant4.wrapping_add(self.evalua_func_bool( suma_ant4,suma_ant4,suma_ant4,suma_ant4,suma_ant4)).wrapping_add(iv4);
		suma_ant5 = suma_ant5.wrapping_add(self.evalua_func_bool( suma_ant5,suma_ant5,suma_ant5,suma_ant5,suma_ant5)).wrapping_add(iv7);
		
		let mask1:i64 = 0xffffffff as i64;
		let mask2 = -0x1 as i64;
		
		let mut hash: i64 = ((suma_ant1 << 48) & mask2 ) |
	               ((suma_ant1.wrapping_add(suma_ant2) << 32) & mask2 ) |
	               ((suma_ant1.wrapping_add(suma_ant2).wrapping_add(suma_ant3) << 16) & mask1) |
	               ((suma_ant3.wrapping_add(suma_ant4).wrapping_add(suma_ant5)) & mask1);

		let rounds = (length as i64)+5+2+1;
		hash = hash.wrapping_add(self.evalua_func_bool( hash,hash,hash,hash,hash).wrapping_add(iv5));
		hash = hash.wrapping_add(self.evalua_func_bool( hash,hash,hash,hash,hash).wrapping_add(iv7).wrapping_add(rounds));

		*hash1 = hash;
	}

}

fn main() {
	let data = "";
	let length = data.len();
    let tail = length % 64;
	let padding;
    if 64 - tail >= 9 {
        padding = 64 - tail;
    } else {
        padding = 128 - tail;
    }
		
	let mut output: Vec<i8> = vec![0;length + padding];
	
	let mut hash1: i64 = 0; 

    let hash = Azrael64{};
	hash.get_hash_eval( data.as_bytes(),&mut output,length,padding,&mut hash1 );
	let x = format!("{:x}", hash1);
	println!("{}",x);
}
	