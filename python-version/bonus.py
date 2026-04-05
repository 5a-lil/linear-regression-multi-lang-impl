import csv
import json
import os

THETA_FILE = "thetas.json"
DATASET_FILE = "../data.csv"


def load_data(filepath):
    mileages = []
    prices = []
    with open(filepath, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            mileages.append(float(row["km"]))
            prices.append(float(row["price"]))
    return mileages, prices


def load_thetas():
    if not os.path.exists(THETA_FILE):
        return 0.0, 0.0
    with open(THETA_FILE, "r") as f:
        data = json.load(f)
    return data["theta0"], data["theta1"]


def estimate_price(mileage, theta0, theta1):
    return theta0 + (theta1 * mileage)


def plot(mileages, prices, theta0, theta1):
    try:
        import matplotlib.pyplot as plt
    except ImportError:
        print("matplotlib not installed. Run: pip install matplotlib")
        return

    # Regression line: two points are enough
    x_line = [min(mileages), max(mileages)]
    y_line = [estimate_price(x, theta0, theta1) for x in x_line]

    plt.figure(figsize=(9, 6))
    plt.scatter(mileages, prices, color="steelblue", alpha=0.7, label="Data points")
    plt.plot(x_line, y_line, color="tomato", linewidth=2, label="Linear regression")
    plt.xlabel("Mileage (km)")
    plt.ylabel("Price (€)")
    plt.title("Car price vs Mileage")
    plt.legend()
    plt.tight_layout()
    plt.show()


def precision(mileages, prices, theta0, theta1):
    m = len(mileages)
    predictions = [estimate_price(km, theta0, theta1) for km in mileages]

    # Mean Absolute Error
    mae = sum(abs(predictions[i] - prices[i]) for i in range(m)) / m

    # Mean Squared Error
    mse = sum((predictions[i] - prices[i]) ** 2 for i in range(m)) / m

    # Root Mean Squared Error
    rmse = mse ** 0.5

    # R² score (coefficient of determination)
    mean_price = sum(prices) / m
    ss_tot = sum((prices[i] - mean_price) ** 2 for i in range(m))
    ss_res = sum((prices[i] - predictions[i]) ** 2 for i in range(m))
    r2 = 1 - (ss_res / ss_tot) if ss_tot != 0 else 0

    print("=== Model Precision ===")
    print(f"MAE  (Mean Absolute Error)       : {mae:.2f} €")
    print(f"MSE  (Mean Squared Error)         : {mse:.2f}")
    print(f"RMSE (Root Mean Squared Error)    : {rmse:.2f} €")
    print(f"R²   (Coefficient of determination): {r2:.4f}  ({r2 * 100:.2f}%)")


def main():
    mileages, prices = load_data(DATASET_FILE)
    theta0, theta1 = load_thetas()

    if theta0 == 0.0 and theta1 == 0.0:
        print("Warning: thetas are 0. Run train.py first for meaningful results.")

    precision(mileages, prices, theta0, theta1)
    plot(mileages, prices, theta0, theta1)


if __name__ == "__main__":
    main()
