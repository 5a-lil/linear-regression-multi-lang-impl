use csv::Reader;
use plotters::prelude::*;
use std::io::BufRead;
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

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let file_path = "../data.csv";
    
    let mut rdr = Reader::from_path(file_path)?;
    let mut value_i: Vec<f64> = vec![];
    let mut value_j: Vec<f64> = vec![];
    
    for result in rdr.records() {
        let record = result?;
        value_i.push(record[0].trim().parse::<f64>()?);
        value_j.push(record[1].trim().parse::<f64>()?);
    }

    let i_max = value_i.iter().fold(f64::NEG_INFINITY, |a, &b| a.max(b));

    let (mut theta0, mut theta1) = (0.0, 0.0);
    if let Ok(thetas) = read_thetas(".thetas") {
        theta0 = thetas.0;
        theta1 = thetas.1;
    } else {
        eprintln!("{}", "No \".thetas\" file found yet, need for model training".bright_white().bold());
    };

    let root = BitMapBackend::new("plot.png", (800, 600)).into_drawing_area();
    root.fill(&WHITE)?;
    
    let mut chart = ChartBuilder::on(&root)
        .caption("y = x²", ("Arial", 30))
        .margin(10)
        .x_label_area_size(40)
        .y_label_area_size(40)
        .build_cartesian_2d(0.0..240000.0, 0.0..9000.0)?;
    
    chart.configure_mesh().draw()?;
    
    chart.draw_series(
        value_i.iter().zip(value_j).map(|(x, y)| Cross::new((*x, y), 4, &BLUE))
    )?;

    let points = vec![(0.0, theta0), (i_max, theta1 * i_max + theta0)];
    chart.draw_series(LineSeries::new(points, &RED))?;

    println!("{}", "Result plot saved in path \"plot.png\"".bright_white().bold());

    Ok(())
}
