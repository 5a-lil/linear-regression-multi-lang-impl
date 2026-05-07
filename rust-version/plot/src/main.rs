use plotters::prelude::*;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let root = BitMapBackend::new("line.png", (800, 600)).into_drawing_area();
    root.fill(&WHITE)?;
    
    let mut chart = ChartBuilder::on(&root)
        .caption("y = x²", ("Arial", 30))
        .margin(10)
        .x_label_area_size(40)
        .y_label_area_size(40)
        .build_cartesian_2d(0.0..3.0, 0.0..10.0)?;
    
    chart.configure_mesh().draw()?;
    
    let points = vec![(0.0, 0.0), (1.0, 1.0), (2.0, 4.0), (3.0, 9.0)];
    chart.draw_series(LineSeries::new(points, &RED))?;

    Ok(())
}
