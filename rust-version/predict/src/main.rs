use std::io::{BufRead, Write, stdin, stdout};
use std::error::Error;
use std::fs::File;
use std::io::{BufReader};
use owo_colors::OwoColorize;

fn read_thetas(file_path: &str) -> Result<(f64, f64), Box<dyn Error>> {
    let file = File::open(file_path)?;
    let reader = BufReader::new(file);
    let (mut theta0, mut theta1) = (0.0, 0.0);

    for line in reader.lines() {
        let line = line?;
        if line.starts_with("theta0:") {
            theta0 = line.split(':').nth(1).unwrap().trim().parse()?;
        } else if line.starts_with("theta1:") {
            theta1 = line.split(':').nth(1).unwrap().trim().parse()?;
        }
    }

    Ok((theta0, theta1))
}

fn main() -> Result<(), Box<dyn Error>> {
    let (theta0, theta1) = read_thetas(".thetas")?;

    let mut input = String::new();
    print!("{}", "Input mileage: ".blue().bold());
    stdout().flush()?;
    stdin().read_line(&mut input)?;
    let mileage: f64 = input.trim().parse()?;

    let price = theta0 + theta1 * mileage;
    println!("{}", format!("km: {mileage} -> price: {price}").bright_white().bold());

    Ok(())
}
