use csv::Reader;
use std::fs::File;
use std::io::{BufWriter, Write};
use std::path::Path;

const ITERS: i32 = 1000;
const LEARN_RATE: f64 = 0.1;

#[derive(Debug)]
struct Data {
    theta0: f64,
    theta1: f64,
    m: usize,
    learn_rate: f64,
    value_i: Vec<f64>,
    value_j: Vec<f64>,
}

impl Data {
    fn new() -> Self {
        Data { 
            theta0: 0.0,
            theta1: 0.0,
            m: 0,
            learn_rate: LEARN_RATE,
            value_i: vec![],
            value_j: vec![],
        }
    }

    fn estimated_price(&self, x: f64) -> f64 {
        self.theta0 + self.theta1 * x
    }

    fn calc_new_thetas(&mut self) {
        let mut sum_estimated_price_theta0: f64 = 0.0;
        for i in 0..self.m {
            sum_estimated_price_theta0 += self.estimated_price(self.value_i[i]) - self.value_j[i];
        }

        let mut sum_estimated_price_theta1: f64 = 0.0;
        for i in 0..self.m {
            sum_estimated_price_theta1 += (self.estimated_price(self.value_i[i]) - self.value_j[i]) * self.value_i[i];
        }

        let theta0_var: f64 = self.learn_rate * (sum_estimated_price_theta0 / self.m as f64);
        let theta1_var: f64 = self.learn_rate * (sum_estimated_price_theta1 / self.m as f64);
        self.theta0 -= theta0_var;
        self.theta1 -= theta1_var;
    }

    fn save_thetas(&self) -> Result<(), Box<dyn std::error::Error>> {
        let output_file_path = Path::new(".thetas");
        let file = File::create(output_file_path)?;
        let mut writer = BufWriter::new(file);
        writeln!(writer, "theta0: {}", self.theta0)?;
        writeln!(writer, "theta1: {}", self.theta1)?;
        println!("theta0 and theta1 saved to \".thetas\" file");
        Ok(())
    }
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut data: Data = Data::new();

    let file_path = "../data.csv";
    
    let mut rdr = Reader::from_path(file_path)?;
    
    for result in rdr.records() {
        let record = result?;
        data.value_i.push(record[0].trim().parse::<f64>()?);
        data.value_j.push(record[1].trim().parse::<f64>()?);
    }

    data.m = data.value_i.len();

    let i_min = data.value_i.iter().fold(f64::INFINITY, |a, &b| a.min(b));
    let i_max = data.value_i.iter().fold(f64::NEG_INFINITY, |a, &b| a.max(b));
    
    let j_min = data.value_j.iter().fold(f64::INFINITY, |a, &b| a.min(b));
    let j_max = data.value_j.iter().fold(f64::NEG_INFINITY, |a, &b| a.max(b));

    data.value_i = data.value_i.iter()
        .map(|&x| (x - i_min) / (i_max - i_min))
        .collect();
    
    data.value_j = data.value_j.iter()
        .map(|&y| (y - j_min) / (j_max - j_min))
        .collect();

    for _ in 0..ITERS {
        data.calc_new_thetas();
    }

    data.theta1 = (j_max - j_min) * data.theta1 / (i_max - i_min);
    data.theta0 = j_min + (j_max - j_min) * data.theta0 - data.theta1 * i_min;
    
    data.save_thetas()
}