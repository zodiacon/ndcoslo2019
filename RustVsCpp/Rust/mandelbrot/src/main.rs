extern crate num_complex;
extern crate png;

use num_complex::Complex;
use std::fs::File;
use std::io::BufWriter;
//use png::HasParameters;

fn main() {
    let from = Complex::new(-1.75, -1.0);
    let to = Complex::new(0.75, 1.0);

    let (width, height) = (640, 480);
    let image = build_mandelbrot(&from, &to, width, height);
    save_image("image.png", &image, width as u32, height as u32).expect("failed to write image");
}

fn build_mandelbrot(from : &Complex<f64>, to: &Complex<f64>, width: i32, height: i32) -> Vec<u8> {
    let mut image = Vec::new();
    image.reserve(width as usize * height as usize);

    let size = to - from;

    for y in 0..height {
        for x in 0..width {
            let c = from + Complex::new(x as f64 * size.re / width as f64, y as f64 * size.im / height as f64);
            let color = mandelbrot_color(&c);
            image.push(color);
        }
    }

    image
}

fn mandelbrot_color(c : &Complex<f64>) -> u8 {
    const ITERATIONS : i32 = 1000;
    let mut z = Complex::new(0.0, 0.0);

    for _ in 0..ITERATIONS {
        z = z * z + c;
        if z.norm_sqr() > 4.0 {
            break;
        }
    }
    if z.norm_sqr() > 4.0 { 0xff } else { 0 }
}

fn save_image(filename : &str, data: &Vec<u8>, width: u32, height: u32) -> Result<(), png::EncodingError> {
    let file = File::create(filename).unwrap();
    let ref mut w = BufWriter::new(file);

    let mut encoder = png::Encoder::new(w, width, height);
    encoder.set_color(png::ColorType::Grayscale);
    encoder.set_depth(png::BitDepth::Eight);
    let mut writer = encoder.write_header().unwrap();

    writer.write_image_data(data.as_slice())
}