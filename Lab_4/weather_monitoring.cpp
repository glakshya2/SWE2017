#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>

const int NUM_SENSORS = 9;
const int DATA_PER_SENSOR = 3;

void process_region_data(const std::vector<float>& data, float& avg_temp, float& avg_humidity, float& max_wind_speed) {
    int num_data_points = data.size() / DATA_PER_SENSOR;

    float temp_sum = 0.0, humidity_sum = 0.0;
    max_wind_speed = -1;

    for (int i = 0; i < num_data_points; ++i) {
        temp_sum += data[i * DATA_PER_SENSOR];
        humidity_sum += data[i * DATA_PER_SENSOR + 1];
        max_wind_speed = std::max(max_wind_speed, data[i * DATA_PER_SENSOR + 2]);
    }

    avg_temp = temp_sum / num_data_points;
    avg_humidity = humidity_sum / num_data_points;
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int regions_per_processor = NUM_SENSORS / size;
    int remaining_regions = NUM_SENSORS % size;

    std::vector<float> full_data;
    if (rank == 0) {
        full_data = {25.5, 65.3, 12.5, 26.1, 64.2, 14.2,
                     30.2, 55.4, 20.1, 28.9, 52.3, 15.4,
                     18.6, 72.1, 9.5, 21.4, 68.8, 10.2};

    }

    int num_sensors = NUM_SENSORS;
    MPI_Bcast(&num_sensors, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<float> local_data(regions_per_processor * DATA_PER_SENSOR);
    MPI_Scatter(full_data.data(), regions_per_processor * DATA_PER_SENSOR, MPI_FLOAT,
                local_data.data(), regions_per_processor * DATA_PER_SENSOR, MPI_FLOAT,
                0, MPI_COMM_WORLD);

    float avg_temp = 0.0, avg_humidity = 0.0, max_wind_speed = 0.0;
    process_region_data(local_data, avg_temp, avg_humidity, max_wind_speed);

    std::vector<float> gathered_data(size * 3); // 3 values (avg temp, avg humidity, max wind speed) per processor
    float local_results[3] = {avg_temp, avg_humidity, max_wind_speed};
    MPI_Gather(local_results, 3, MPI_FLOAT, gathered_data.data(), 3, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        float global_avg_temp = 0.0, global_avg_humidity = 0.0, global_max_wind_speed = -1.0;
        for (int i = 0; i < size; ++i) {
            global_avg_temp += gathered_data[i * 3];
            global_avg_humidity += gathered_data[i * 3 + 1];
            global_max_wind_speed = std::max(global_max_wind_speed, gathered_data[i * 3 + 2]);
        }
        global_avg_temp /= size;
        global_avg_humidity /= size;

        std::cout << "Global Weather Report:\n";
        std::cout << "Avg Temperature: " << global_avg_temp << "\n";
        std::cout << "Avg Humidity: " << global_avg_humidity << "\n";
        std::cout << "Max Wind Speed: " << global_max_wind_speed << "\n";
    }

    float global_report[3];
    MPI_Allgather(local_results, 3, MPI_FLOAT, global_report, 3, MPI_FLOAT, MPI_COMM_WORLD);

    if (rank != 0) {
        std::cout << "Processor " << rank << " received global weather report:\n";
        std::cout << "Avg Temperature: " << global_report[0] << "\n";
        std::cout << "Avg Humidity: " << global_report[1] << "\n";
        std::cout << "Max Wind Speed: " << global_report[2] << "\n";
    }

    MPI_Finalize();
    return 0;
}
