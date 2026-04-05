import csv
import json

THETA_FILE = "thetas.json"
DATASET_FILE = "../data.csv"

LEARNING_RATE = 0.1
ITERATIONS = 1000


def load_data(filepath):
    mileages = []
    prices = []
    with open(filepath, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            mileages.append(float(row["km"]))
            prices.append(float(row["price"]))
    return mileages, prices


def normalize(values):
    min_v = min(values)
    max_v = max(values)
    normalized = [(v - min_v) / (max_v - min_v) for v in values]
    return normalized, min_v, max_v


def estimate_price(mileage, theta0, theta1):
    return theta0 + (theta1 * mileage)


def train(mileages, prices, learning_rate, iterations):
    theta0 = 0.0
    theta1 = 0.0
    m = len(mileages)

    for _ in range(iterations):
        tmp0 = learning_rate * (1 / m) * sum(
            estimate_price(mileages[i], theta0, theta1) - prices[i]
            for i in range(m)
        )
        tmp1 = learning_rate * (1 / m) * sum(
            (estimate_price(mileages[i], theta0, theta1) - prices[i]) * mileages[i]
            for i in range(m)
        )
        theta0 -= tmp0
        theta1 -= tmp1

    return theta0, theta1


def save_thetas(theta0, theta1):
    with open(THETA_FILE, "w") as f:
        json.dump({"theta0": theta0, "theta1": theta1}, f)


def main():
    mileages, prices = load_data(DATASET_FILE)

    # Normalize to help gradient descent converge
    norm_mileages, min_km, max_km = normalize(mileages)
    norm_prices, min_price, max_price = normalize(prices)

    theta0, theta1 = train(norm_mileages, norm_prices, LEARNING_RATE, ITERATIONS)

    # Denormalize thetas back to original scale
    # price = theta0 * (max_p - min_p) + min_p  +  theta1 * (max_p - min_p) / (max_km - min_km) * km
    #       - theta1 * (max_p - min_p) / (max_km - min_km) * min_km
    real_theta1 = theta1 * (max_price - min_price) / (max_km - min_km)
    real_theta0 = theta0 * (max_price - min_price) + min_price - real_theta1 * min_km

    save_thetas(real_theta0, real_theta1)
    print(f"Training complete.")
    print(f"theta0 = {real_theta0:.4f}")
    print(f"theta1 = {real_theta1:.6f}")


if __name__ == "__main__":
    main()
