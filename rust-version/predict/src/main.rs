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
    let (mut theta0, mut theta1) = (0.0, 0.0);

    if let Ok(thetas) = read_thetas(".thetas") {
        theta0 = thetas.0;
        theta1 = thetas.1;
    } else {
        eprintln!("{}", "No \".thetas\" file found yet, need for model training".bright_white().bold());
    };

    let mut input = String::new();
    print!("{}", "Input mileage: ".blue().bold());
    stdout().flush()?;
    if let Err(_) = stdin().read_line(&mut input) {
        eprintln!("{}", "Error while reading input from stdin".bright_red().bold());
    };

    if let Ok(mileage) = input.trim().parse::<f64>() {
        let price = theta0 + theta1 * mileage;
        println!("{}", format!("km: {mileage} -> price: {price}").bright_white().bold());
    } else {
        eprintln!("{}", "Error while parsing numeric supposed input".bright_red().bold());
    };

    Ok(())
}
