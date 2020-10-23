declare var require: any;

const Integer = require('integer');

function longToBytes( l:typeof Integer ) {
	let result = new Array<number>(8);
    for (let i = 7; i >= 0; i--) {
        result[i] = l.and(Integer(0xFF));
        l = l.shiftRight(8);
    }
    return result;
}


function add( x: typeof Integer, y: typeof Integer ) {
	try {
		return x.add(y);
	} catch( error ) {
		let w: typeof Integer;
		let ww: typeof Integer;
		let www: typeof Integer;
		if( y.lessThan(x) ) {
			if( x.lessThan( Integer(0) ) ) {
				w = Integer.MIN_VALUE.subtract(y).abs();
				ww = x.abs().subtract(w);
				www = Integer.MAX_VALUE.subtract(ww).add(Integer(1));
			} else {
				w = Integer.MAX_VALUE.subtract(x);
				ww = y.abs().subtract(w).abs();
				www = Integer.MIN_VALUE.add(ww).subtract(Integer(1));
			}
		} else {
			if( y.lessThan( Integer(0) ) ) {
				w = Integer.MIN_VALUE.subtract(x).abs();
				ww = y.abs().subtract(w);
				www = Integer.MAX_VALUE.subtract(ww).add(Integer(1));
			} else {
				w = Integer.MAX_VALUE.subtract(y);
				ww = x.abs().subtract(w).abs();
				www = Integer.MIN_VALUE.add(ww).subtract(Integer(1));
			}
		}
		return www;
	}
}

function toUTF8Array(str: string) {
    let utf8 = [];
    for (let i=0; i < str.length; i++) {
        let charcode = str.charCodeAt(i);
        if (charcode < 0x80) utf8.push(charcode);
        else if (charcode < 0x800) {
            utf8.push(0xc0 | (charcode >> 6), 
                      0x80 | (charcode & 0x3f));
        }
        else if (charcode < 0xd800 || charcode >= 0xe000) {
            utf8.push(0xe0 | (charcode >> 12), 
                      0x80 | ((charcode>>6) & 0x3f), 
                      0x80 | (charcode & 0x3f));
        }
        // surrogate pair
        else {
            i++;
            // UTF-16 encodes 0x10000-0x10FFFF by
            // subtracting 0x10000 and splitting the
            // 20 bits of 0x0-0xFFFFF into two halves
            charcode = 0x10000 + (((charcode & 0x3ff)<<10)
                      | (str.charCodeAt(i) & 0x3ff));
            utf8.push(0xf0 | (charcode >>18), 
                      0x80 | ((charcode>>12) & 0x3f), 
                      0x80 | ((charcode>>6) & 0x3f), 
                      0x80 | (charcode & 0x3f));
        }
    }
    return utf8;
}

function evaluaFuncBool(x1:typeof Integer, x2:typeof Integer, x3:typeof Integer, x4:typeof Integer, x5:typeof Integer) {
		let c_0:typeof Integer = (add(x1,x2).xor(x3.xor(x4))).xor(x5);
		let c_1:typeof Integer = ((x1.and(x2)).xor(add(x3,x4))).xor(x5);
		let c_2:typeof Integer = (add((x1.xor(x2)),(add(x3,x4)))).xor(x5);
		let c_3:typeof Integer = ((x1.xor(x2)).xor(add(x3,x4))).xor(x5);
		let c_4:typeof Integer = add(add((x1.and(x2)),(add(x3,x4))),x5);
		let c_5:typeof Integer = ( add((x1.and(x2)),add(x3,x4)) ).xor(x5);
		let c_6:typeof Integer = ((x1.xor(x2)).xor(add(x3,x4))).xor(x5);
		let c_7:typeof Integer = (( x1.or(x2) ).xor( add(x3,x4) )).xor(x5);
		let c_8:typeof Integer = (  x1.or(x2) ).or(  add(x3,x4).xor(x5) );
		let c_9:typeof Integer = ( add((add(x1,x2)),(add(x3,x4))) ).xor(x5);
		let c_a:typeof Integer = ( (add(x1,x2)).and( add(x3,x4) )).xor(x5);
		let c_b:typeof Integer = ( add((x1.xor(x2)),( x3.xor(x4) )) ).xor(x5);
		let c_c:typeof Integer = (( x1.or(x2) ).xor( x3.xor(x4) )).xor(x5);
		let c_d:typeof Integer = ( add((  add(x1,x2) ),( x3.xor(x4) )) ).xor(x5);
		let c_e:typeof Integer = ( (  add(x1,x2) ).xor( add(x3,x4)) ).xor(x5);
		let c_f:typeof Integer = add(x1,x2).and(add(add(x3,x4),x5));
		return add(add(add(add(add(add(add(add(add(add(add(add(add(add(add(c_0,c_1),c_2),c_3),c_4),c_5),c_6),c_7),c_8),c_9),c_a),c_b),c_c),c_d),c_e),c_f);
}


class Azrael64 {
	
	pad( data: Array<number> ) {
	    let length = data.length;
	    let tail = length % 64;
		let padding; 
	    if (64 - tail >= 9) {
	        padding = 64 - tail;
	    } else {
	        padding = 128 - tail;
		}
	
	    let pad = new Array<typeof Integer>(padding);
	    pad[0] = Integer(-0x80);
	    let bits = length * 8;
		for( let i=1; i<=padding-1-8; i++ ) {
			pad[i]= Integer(0);
		}
	    for( let i=0; i<8; i++ ) {
	        pad[padding - 1 - i] = Integer((bits >> (8 * i)) & 0xFF);
		}
	
		let out = new Array<typeof Integer>(length+padding);
	    for( let i=0; i<length; i++ ) {
	        out[i] = Integer(data[i]);
		}
		for( let i=0; i<padding; i++ ) {
	        out[length+i] = pad[i];
		}
	    return out;
	}
	
	getHashEval( input1: Array<number> ) {
		
		let input = this.pad(input1);
		
		let IV1 = Integer.fromString("6a09e667bb67ae85",16);
		let IV2 = Integer.fromString("3c6ef372a54ff53a",16);
		let IV3 = Integer.fromString("510e527f9b05688c",16);
		let IV4 = Integer.fromString("1f83d9ab5be0cd19",16);
		let IV5 = Integer.fromString("428a2f9871374491",16);
		let IV6 = Integer.fromString("-4A3F0430164A245B",16);
		let IV7 = Integer.fromString("3956c25b59f111f1",16);
		let IV8 = Integer.fromString("-6DC07D5B54E3A12B",16);
		let IV9 = Integer.fromString("-27F85567ED7CA4FF",16);
		let IV10 = Integer.fromString("243185be550c7dc3",16);

		let char1:typeof Integer =IV1;
		let char2:typeof Integer =IV2;
		let char3:typeof Integer =IV3;
		let char4:typeof Integer =IV4;
		let char5:typeof Integer =IV5;
		
		let sumaAnt1:typeof Integer = IV6;
		let sumaAnt2:typeof Integer = IV7;
		let sumaAnt3:typeof Integer = IV8;
		let sumaAnt4:typeof Integer = IV9;
		let sumaAnt5:typeof Integer = IV10;
		
		char1 = add(char1,input[ input.length-2 ]);
		char2 = add(char2,input[ input.length-1 ]);
		char3 = add(char3,input[ 0 ]);
		char4 = add(char4,input[ 1 ]);
		char5 = add(char5,input[ 2 ]);

		sumaAnt5 = add(sumaAnt5,sumaAnt4);
		sumaAnt4 = add(sumaAnt4,sumaAnt3);
		sumaAnt3 = add(sumaAnt3,sumaAnt2);
		sumaAnt2 = add(sumaAnt2,sumaAnt1);
		sumaAnt1 = add(sumaAnt1,evaluaFuncBool( char1,char2,char3,char4,char5));
		
		// Main Loop.
		for( let i=1; i<input.length-1; i++ ) {
			char1 = add(char1,sumaAnt1);
			char2 = add(char2,char3);
			char3 = add(char3,char4);
			char4 = add(char4,input[ i+1 ]);
			char5 = add(char5,sumaAnt2);
			sumaAnt5 = add(sumaAnt5,sumaAnt4);
			sumaAnt4 = add(sumaAnt4,sumaAnt3);
			sumaAnt3 = add(sumaAnt3,sumaAnt2);
			sumaAnt2 = add(sumaAnt2,sumaAnt1);
			sumaAnt1 = add(sumaAnt1,evaluaFuncBool( char1,char2,char3,char4,char5));
		}
		
		char1 = add(char1,sumaAnt1);
		char2 = add(char2,char3);
		char3 = add(char3,char4);
		char4 = add(char4,input[ 0 ]);
		char5 = add(char5,sumaAnt2);
		sumaAnt5 = add(sumaAnt5,sumaAnt4);
		sumaAnt4 = add(sumaAnt4,sumaAnt3);
		sumaAnt3 = add(sumaAnt3,sumaAnt2);
		sumaAnt2 = add(sumaAnt2,sumaAnt1);
		sumaAnt1 = add(sumaAnt1,evaluaFuncBool( char1,char2,char3,char4,char5));

		sumaAnt1 = add(add(sumaAnt1,evaluaFuncBool( sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1,sumaAnt1)),IV1);
		sumaAnt2 = add(add(sumaAnt2,evaluaFuncBool( sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2,sumaAnt2)),IV2);
		sumaAnt3 = add(add(sumaAnt3,evaluaFuncBool( sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3,sumaAnt3)),IV3);
		sumaAnt4 = add(add(sumaAnt4,evaluaFuncBool( sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4,sumaAnt4)),IV4);
		sumaAnt5 = add(add(sumaAnt5,evaluaFuncBool( sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5,sumaAnt5)),IV7);
		
		let mask1:typeof Integer = Integer(0xffffffff);
		let mask2:typeof Integer = Integer(-0x1);
		
		let hash:typeof Integer = ((sumaAnt1.shiftLeft(48)).and(mask2) ).or(
	               ((add(sumaAnt1,sumaAnt2).shiftLeft(32)).and(mask2) ) ).or(
	               ((add(add(sumaAnt1,sumaAnt2),sumaAnt3).shiftLeft(16)).and(mask1)) ).or(
	               ((add(add(sumaAnt3,sumaAnt4),sumaAnt5)).and(mask1)) );

		let rounds:typeof Integer = input.length+5+2+1;

		hash = add(add(hash,evaluaFuncBool( hash,hash,hash,hash,hash)),IV5);
		hash = add(add(add(hash,evaluaFuncBool( hash,hash,hash,hash,hash)),IV7),rounds);

		return hash;
		
	}
}

const hash = new Azrael64();
let hash1:typeof Integer = hash.getHashEval( toUTF8Array("") );
let hex = "";
let bites = longToBytes(hash1);
for (let i = 0; i<8; i++) {
	hex += bites[i].toString(16);
}
console.log(hex);
