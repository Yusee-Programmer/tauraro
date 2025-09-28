use sha1::Sha1;
use sha2::{Sha224, Sha256, Sha384, Sha512, Digest};
use sha3::{Sha3_224, Sha3_256, Sha3_384, Sha3_512};
use md5::Md5;

fn main() {
    let data = b"hello world";
    
    // Test MD5
    let mut md5_hasher = Md5::new();
    md5_hasher.update(data);
    let md5_result = md5_hasher.finalize();
    println!("MD5: {:x}", md5_result);
    
    // Test SHA1
    let mut sha1_hasher = Sha1::new();
    sha1_hasher.update(data);
    let sha1_result = sha1_hasher.finalize();
    println!("SHA1: {:x}", sha1_result);
    
    // Test SHA256
    let mut sha256_hasher = Sha256::new();
    sha256_hasher.update(data);
    let sha256_result = sha256_hasher.finalize();
    println!("SHA256: {:x}", sha256_result);
    
    println!("Crypto libraries work correctly!");
}