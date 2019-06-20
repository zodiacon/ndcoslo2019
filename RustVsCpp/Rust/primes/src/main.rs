use std::thread;
use std::sync::{Arc,Mutex};
use std::time::SystemTime;


fn main() {
    let args = std::env::args();
    let nthreads = if args.len() > 1 { 
        i32::from_str_radix(&args.last().unwrap(), 10).unwrap() 
    } 
    else { 
        4 
    };

    println!("Using {} threads", nthreads);
    let start = SystemTime::now();

    let primes = calc_primes_fixed(nthreads, 3, 20000000);
    let end = SystemTime::now();

    println!("Total primes: {} in {} msec", primes, end.duration_since(start).unwrap().as_millis());
}

fn calc_primes_fixed(n: i32, first: i32, last: i32) -> usize {
    let data = Vec::new();
    let lock = Arc::new(Mutex::new(data));

    let mut handles  = Vec::new();
    let chunk = (last - first + 1) / n;

    for i in 0..n {
        let start = chunk * i + first;
        let end = if i == n - 1 { last } else { start + chunk - 1};

        let clone = lock.clone();
        let handle = thread::spawn(move || {
            for i in start..=end {
                if is_prime(i) {
                     let mut v = clone.lock().unwrap();
                     v.push(i);
                }
            }
        });
        handles.push(handle);
    }
    
    for h in handles {
        h.join().unwrap();
    }

    let data = lock.lock().unwrap();
    data.len()
}

fn is_prime(n: i32) -> bool {
    let limit = (n as f32).sqrt() as i32;
    for i in 2..=limit {
        if n % i == 0 {
            return false;
        }

    }
    true
}

// fn calc_primes(n: i32, first: i32, last: i32) -> usize {
//     let result = Mutex::new(Vec::new());

//     let mut handles  = Vec::new();
//     let chunk = (last - first + 1) / n;
//     let i = 0;

//     //for i in 0..n {
//         let start = chunk * i + first;
//         let end = if i == n - 1 { last } else { start + chunk - 1};
//         let handle = thread::spawn( move || {
//             for i in start..=end {
//                 if is_prime(i) {
//                      result.lock().unwrap().push(i);
//                 }
//             }
//         });
//         handles.push(handle);
//     //}
    
//     for h in handles {
//         h.join().unwrap();
//     }

  
//     let data = result.lock().unwrap();
//     data.len()
// }
