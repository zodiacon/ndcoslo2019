use std::f32;

fn main() {
    println!("Hello, Turtle!");

    let mut t = Turtle::new();
    print_turtle(&t);
    t.execute(TurtleCommand::Forward(100.0));
    print_turtle(&t);
    t.execute(TurtleCommand::RotateRight);
    print_turtle(&t);
    t.execute(TurtleCommand::Forward(50.0));
    print_turtle(&t);
    t.execute(TurtleCommand::Rotate(-45.0));
    print_turtle(&t);
    t.execute(TurtleCommand::Forward(100.0));
    print_turtle(&t);
    t.execute(TurtleCommand::RotateLeft);
    print_turtle(&t);
    t.execute(TurtleCommand::PenColor(Color { r: 255, g: 0, b: 0 }));
    print_turtle(&t);
    t.execute(TurtleCommand::Backwards(60.0));
    print_turtle(&t);
}

fn print_turtle(t: &Turtle) {
    println!("Heading: {}, X: {}, Y: {} Color: {:?}", t.heading, t.x, t.y, t.pen_color);
}

#[derive(Debug)]
struct Color {
    r: u8,
    g: u8,
    b: u8
}

enum TurtleCommand {
	Forward(f32),
	Backwards(f32),
	Rotate(f32),
	RotateRight,
	RotateLeft,
	PenColor(Color)
}

struct Turtle {
    heading: f32,
    x: f32,
    y: f32,
    pen_color: Color
}

impl Turtle {
    pub fn new() -> Turtle {
        Turtle { 
            heading: 0f32, 
            x: 0f32, y: 0f32, 
            pen_color: Color { r: 255, b: 255, g: 255 } 
        }
    }

    pub fn execute(&mut self, command: TurtleCommand) {
        match command {
            TurtleCommand::RotateRight => self.heading += 90f32,
            TurtleCommand::RotateLeft => self.heading -= 90f32,
            TurtleCommand::Rotate(angle) => self.heading += angle,
            TurtleCommand::Forward(dist) => {
                self.x += dist * f32::cos(self.heading * f32::consts::PI / 180f32);
                self.y += dist * f32::sin(self.heading * f32::consts::PI / 180f32);
            },
            TurtleCommand::Backwards(dist) => self.execute(TurtleCommand::Forward(-dist)),
            TurtleCommand::PenColor(color) => self.pen_color = color,
            
        }
    }
}
