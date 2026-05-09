# Linear Regression - Multi-Language Implementation

A complete linear regression project implementing gradient descent from scratch in **C**, **Python**, and **Rust**. Each language version demonstrates different levels of complexity, from bare-metal memory management to high-level ergonomics with visualization.

## 📋 Overview

This project predicts numerical values using linear regression with gradient descent optimization. Each implementation:

- Reads a CSV dataset (mileage → price)
- Normalizes features for stable convergence
- Trains the model using batch gradient descent
- Saves the trained parameters
- Provides prediction functionality

## 🚀 Features by Language

| Feature | C | Python | Rust |
|---------|---|--------|------|
| CSV parsing | Custom implementation | `csv` module | `csv` crate |
| Gradient descent | ✅ From scratch | ✅ From scratch | ✅ From scratch |
| Feature normalization | ✅ Manual | ✅ Manual | ✅ Manual |
| Model persistence | ".thetas" file | "thetas.json" file | ".thetas" file |
| Prediction CLI | ✅ | ✅ | ✅ |
| Precision metrics (MAE, MSE, RMSE, R²) | ❌ | ✅ | ✅ |
| Visualization | ❌ | ✅ (matplotlib) | ✅ (plotters) |
| Data denormalization | ✅ | ✅ | ✅ |

## 📁 Project Structure

```
.
├── C/
│   ├── train.c          # Training program (C)
│   └── predict.c        # Prediction program (C)
├── python/
│   ├── train.py         # Training program (Python)
│   ├── predict.py       # Prediction CLI (Python)
│   └── plot.py          # Visualization + precision metrics (Python)
├── rust/
│   ├── train/
│   │   └── src/main.rs      # Training + precision (Rust)
│   ├── predict/
│   │   └── src/main.rs      # Prediction CLI (Rust)
│   └── plot/
│       └── src/main.rs      # Generates regression plot (Rust)
├── data.csv             # Dataset (mileage, price)
└── README.md
```

## 🧠 Mathematical Implementation

### Hypothesis Function
```
h(x) = θ₀ + θ₁ × x
```

### Cost Function (Mean Squared Error)
```
J(θ₀, θ₁) = (1/2m) × Σ(h(xᵢ) - yᵢ)²
```

### Gradient Descent Update Rules
```
θ₀ := θ₀ - α × (1/m) × Σ(h(xᵢ) - yᵢ)
θ₁ := θ₁ - α × (1/m) × Σ(h(xᵢ) - yᵢ) × xᵢ
```

Where α is the learning rate and m is the number of training examples.

## ⚙️ Installation & Usage

### Dataset Format
CSV file with header:
```
km,price
240000,3650
139800,3800
...
```

### C Version

**Compile:**
```
cd c-version
cc -o train train.c
cc -o predict predict.c
```

**Train:**
```
./train ../data.csv
```

**Predict:**
```
./predict <mileage>
```

**Note:** The training program normalizes data, trains for 1000 iterations (default), and saves parameters to `.thetas`.

### Python Version

**Install dependencies:**
```
cd python-version
pip install matplotlib
```

**Train:**
```
python3 train.py
```

**Predict:**
```
python3 predict.py
```

**Plot + Precision:**
```
python3 plot.py
```

**Features:**
- Training: 1000 iterations with 0.1 learning rate
- Output: `thetas.json`
- Precision metrics: MAE, MSE, RMSE, R²
- Interactive plot with matplotlib

### Rust Version

**Build:**
```
cd rust-version
cargo build --release
```

**Train with precision:**
```
PREC=1 cargo run --release --bin train
```

**Predict:**
```
cargo run --release --bin predict
```

**Generate plot:**
```
cargo run --release --bin plot
```

**Features:**
- Training: 1000 iterations with 0.1 learning rate
- Precision metrics (when `PREC=1` is set)
- Colored terminal output
- PNG plot with regression line `plot.png`
- Parameters saved to `.thetas`

## 📊 Precision Metrics (Python & Rust)

| Metric | Description |
|--------|-------------|
| **MAE** | Mean Absolute Error - average error magnitude |
| **MSE** | Mean Squared Error - penalizes large errors |
| **RMSE** | Root Mean Squared Error - error in original units |
| **R²** | Coefficient of determination - variance explained (0-1) |

## 🎯 Implementation Highlights

### C Version
- **Zero dependencies** - only standard library
- **Manual memory management** with dynamic arrays
- **Custom CSV parsing** with validation
- **Robust error handling** with goto cleanup
- **Manual string parsing** for floats
- **Epured line parsing** (removes all whitespace)

The C implementation demonstrates low-level systems programming:
- Manual dynamic array resizing with `realloc`-like logic
- Custom `is_float()` validation using `strtod`
- Line-by-line CSV parsing with `getline()`
- Memory cleanup with `free()` on all paths

### Python Version
- **Clean, readable code** with minimal boilerplate
- **Complete precision metrics** (MAE, MSE, RMSE, R²)
- **Interactive visualization** with matplotlib
- **JSON persistence** for model parameters
- **Normalization/denormalization** for gradient stability
- **Bonus features**: plotting and precision calculation

### Rust Version
- **Multi-crate architecture** (train, predict, plot)
- **Type safety** with strong static typing
- **Zero-cost abstractions** for performance
- **Result-based error handling** (no panics)
- **Feature flags** (PREC env var for optional metrics)
- **Colored terminal output** with `owo-colors`
- **Static dispatch** for performance
- **Memory safety** without garbage collection

## 🔧 Configuration Parameters

| Parameter | C | Python | Rust |
|-----------|-------|--------|------|
| Learning rate | 0.1 | 0.1 | 0.1 |
| Iterations | 1000 | 1000 | 1000 |
| Normalization | Min-Max | Min-Max | Min-Max |

## 📈 Visualization

### Python (`matplotlib`)
- Interactive plot window
- Scatter points + regression line
- Automatic display with `plt.show()`

### Rust (`plotters`)
- PNG output (`plot.png`)
- Clean, publication-quality graphics
- No external runtime required

## 📝 Notes

- All implementations **normalize** input data for numerical stability
- The model uses **batch gradient descent** (full dataset per iteration)
- **No external ML libraries** are used for the algorithm itself (only parsing and plotting helpers)
- The C version uses manual file I/O and parsing (`getline`, `strtod`)
- The Rust version uses the `csv` crate for parsing but the algorithm is 100% hand-written
- The Python version uses built-in `csv` module with custom gradient descent

## 🎓 Learning Outcomes

This project demonstrates:
- Gradient descent implementation from first principles
- Feature normalization importance for convergence
- Manual CSV parsing (C) vs library-assisted parsing (Python/Rust)
- Memory management strategies across languages
- Error handling patterns
- Model persistence strategies
- Visualization integration

# Have fun analyzing and using it 🔥
