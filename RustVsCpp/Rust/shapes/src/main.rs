mod shape;

use shape::Shape;

struct Rectangle {
    width : f32,
    height: f32
}

struct Ellipse {
    radius_x: f32,
    radius_y: f32
}

struct Circle {
    radius: f32
}

impl Shape for Rectangle {
    fn area(&self) -> f32 {
        self.width * self.height
    }

    fn as_string(&self) -> String {
        format!("Rectangle: width={}, height={}", self.width, self.height)
    }
}

impl Shape for Circle {
    fn area(&self) -> f32 {
        self.radius * self.radius * std::f32::consts::PI / 2f32
    }

    fn as_string(&self) -> String {
        format!("Circle: radius={}", self.radius)
    }
}

impl Shape for Ellipse {
    fn area(&self) -> f32 {
        self.radius_x * self.radius_y * std::f32::consts::PI / 2f32
    }

    fn as_string(&self) -> String {
        format!("Ellipse: radii=({},{})", self.radius_x, self.radius_y)
    }
}

fn main() {
    let mut shapes: Vec<&dyn Shape> = vec![];
    
    let r1 = Rectangle { width: 10f32, height: 6f32 };
    shapes.push(&r1);

    let c1 = Circle { radius: 20f32 };
    shapes.push(&c1);

    let e1 = Ellipse { radius_x: 5f32, radius_y: 8f32 };
    shapes.push(&e1);

    let e2 = Ellipse { radius_x: 20f32, radius_y: 20f32 };
    shapes.push(&e2);

    for &s in shapes.iter() {
        println!("{}", s.as_string());
        println!("\tArea: {}", s.area());
    }
}
