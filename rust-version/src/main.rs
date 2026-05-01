use csv::Reader;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    // Path to your CSV file
    let file_path = "../../data.csv";
    
    // Create reader from file
    let mut rdr = Reader::from_path(file_path)?;
    
    // Iterate over records
    for result in rdr.records() {
        let record = result?;
        println!("{:?}", record);
    }
    
    Ok(())
}