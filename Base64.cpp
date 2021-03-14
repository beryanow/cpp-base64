#include <vector>
#include "Base64.h"

const std::string base_64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

std::vector<int> encode_six_bit_bytes(std::string data) {
    int three_bytes_batch_amount;
    int non_batched_bytes_amount = 0;

    int six_bit_byte;
    int batch_first_byte, batch_second_byte, batch_third_byte;

    std::vector<int> six_bit_bytes_vector;

    if (data.size() % 3 != 0) {
        non_batched_bytes_amount = data.size() % 3;
        three_bytes_batch_amount = (data.size() - non_batched_bytes_amount) / 3;
    } else {
        three_bytes_batch_amount = data.size() / 3;
    }

    for (int i = 0; i < three_bytes_batch_amount; i++) {
        batch_first_byte = (int) data[i * 3];
        batch_second_byte = (int) data[i * 3 + 1];
        batch_third_byte = (int) data[i * 3 + 2];

        six_bit_byte = batch_first_byte >> 2;
        six_bit_bytes_vector.push_back(six_bit_byte);

        six_bit_byte = ((batch_first_byte & 3) << 4) | (batch_second_byte >> 4);
        six_bit_bytes_vector.push_back(six_bit_byte);

        six_bit_byte = ((batch_second_byte & 15) << 2) | (batch_third_byte >> 6);
        six_bit_bytes_vector.push_back(six_bit_byte);

        six_bit_byte = batch_third_byte & 63;
        six_bit_bytes_vector.push_back(six_bit_byte);
    }

    if (non_batched_bytes_amount == 1) {
        batch_first_byte = (int) data[data.size() - non_batched_bytes_amount];

        six_bit_byte = batch_first_byte >> 2;
        six_bit_bytes_vector.push_back(six_bit_byte);

        six_bit_byte = (batch_first_byte & 3) << 4;
        six_bit_bytes_vector.push_back(six_bit_byte);

        six_bit_bytes_vector.push_back(64);
        six_bit_bytes_vector.push_back(64);
    }

    if (non_batched_bytes_amount == 2) {
        batch_first_byte = (int) data[data.size() - non_batched_bytes_amount];
        batch_second_byte = (int) data[data.size() - non_batched_bytes_amount + 1];

        six_bit_byte = batch_first_byte >> 2;
        six_bit_bytes_vector.push_back(six_bit_byte);

        six_bit_byte = ((batch_first_byte & 3) << 4) | (batch_second_byte >> 4);
        six_bit_bytes_vector.push_back(six_bit_byte);

        six_bit_byte = (batch_second_byte & 15) << 2;
        six_bit_bytes_vector.push_back(six_bit_byte);

        six_bit_bytes_vector.push_back(64);
    }

    return six_bit_bytes_vector;
}

std::string encode(std::string message) {
    std::vector<int> six_bit_bytes_vector = encode_six_bit_bytes(message);
    int six_bit_bytes_vector_size = six_bit_bytes_vector.size();

    char *result = new char[six_bit_bytes_vector_size];
    for (int i = 0; i < six_bit_bytes_vector_size; i++) {
        result[i] = base_64_table[six_bit_bytes_vector[i]];
    }
    result[six_bit_bytes_vector_size] = '\0';

    return result;
}

std::string decode(std::vector<int> data) {
    int four_bytes_batch_amount;
    int non_batched_bytes_amount = 0;

    int eight_bit_byte;
    int batch_first_byte, batch_second_byte, batch_third_byte, batch_fourth_byte;

    std::vector<int> eight_bit_bytes_vector;
    int data_size = data.size();

    if (data[data_size - 1] == 64) {
        if (data[data_size - 2] == 64) {
            non_batched_bytes_amount = 2;
        } else {
            non_batched_bytes_amount = 1;
        }
        four_bytes_batch_amount = (data_size - non_batched_bytes_amount) / 4;
    } else {
        four_bytes_batch_amount = data_size / 4;
    }

    for (int i = 0; i < four_bytes_batch_amount; i++) {
        batch_first_byte = data[i * 4];
        batch_second_byte = data[i * 4 + 1];
        batch_third_byte = data[i * 4 + 2];
        batch_fourth_byte = data[i * 4 + 3];

        eight_bit_byte = (batch_first_byte << 2) | (batch_second_byte >> 4) & 3;
        eight_bit_bytes_vector.push_back(eight_bit_byte);

        eight_bit_byte = ((batch_second_byte & 15) << 4) | (batch_third_byte >> 2);
        eight_bit_bytes_vector.push_back(eight_bit_byte);

        eight_bit_byte = ((batch_third_byte & 3) << 6) | batch_fourth_byte;
        eight_bit_bytes_vector.push_back(eight_bit_byte);
    }

    if (non_batched_bytes_amount == 2) {
        batch_first_byte = data[data_size - non_batched_bytes_amount - 2];
        batch_second_byte = data[data_size - non_batched_bytes_amount - 1];

        eight_bit_byte = ((batch_first_byte & 63) << 2) | (batch_second_byte >> 4);
        eight_bit_bytes_vector.push_back(eight_bit_byte);
    }

    if (non_batched_bytes_amount == 1) {
        batch_first_byte = data[data_size - non_batched_bytes_amount - 3];
        batch_second_byte = data[data_size - non_batched_bytes_amount - 2];
        batch_third_byte = data[data_size - non_batched_bytes_amount - 1];

        eight_bit_byte = ((batch_first_byte & 63) << 2) | (batch_second_byte >> 4);
        eight_bit_bytes_vector.push_back(eight_bit_byte);

        eight_bit_byte = (((batch_second_byte & 15) << 4)) | (batch_third_byte >> 2);
        eight_bit_bytes_vector.push_back(eight_bit_byte);
    }

    int eight_bit_bytes_vector_size = eight_bit_bytes_vector.size();

    char *result = new char[eight_bit_bytes_vector_size];
    for (int i = 0; i < eight_bit_bytes_vector_size; i++) {
        result[i] = (char) eight_bit_bytes_vector[i];
    }
    result[eight_bit_bytes_vector_size] = '\0';

    return result;
}

std::vector<int> decode_six_bit_bytes(std::string data) {
    std::vector<int> six_bit_bytes_vector;

    for (int i = 0; i < data.size(); i++) {
        int six_bit_byte = 0;
        while (base_64_table[six_bit_byte] != data[i]) {
            six_bit_byte++;
        }

        six_bit_bytes_vector.push_back(six_bit_byte);
    }

    return six_bit_bytes_vector;
}

std::string decode(std::string message) {
    std::vector<int> six_bit_bytes_vector = decode_six_bit_bytes(message);
    message = decode(six_bit_bytes_vector);

    return message;
}