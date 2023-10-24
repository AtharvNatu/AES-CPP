#include "../../include/Common/Helper.hpp"

// Function Definitions
const vector<byte_arr_t> read_data_file(string src_input_file)
{
    // Variable Declarations
    vector<byte_arr_t> byte_data;
    char ch;
    unsigned int counter = 0;
    byte_arr_t next_byte_array;
    ifstream input_file_stream;

    // Code
    input_file_stream.open(src_input_file, ifstream::binary);
    if (!input_file_stream)
    {
        if (DEBUG)
            cerr << endl << "Failed To Open Input File !!!" << endl;
        exit(AES_FAILURE);
    }

    while (!input_file_stream.eof())
    {
        if (counter < AES_LENGTH)
        {
            input_file_stream.get(ch);
            next_byte_array.push_back(ch);
            counter++;
        }
        else
        {
            byte_data.push_back(next_byte_array);
            next_byte_array = {};
            counter = 0;
        }
    }

    input_file_stream.close();
    
    return byte_data;
}

void print_byte(byte_t byte)
{
    // Code
    cout << endl << "Byte : " << std::hex << (int) byte;
}

void print_byte_array(byte_arr_t &arr)
{
    // Code
    for (size_t i = 0; i < arr.size(); i++)
        cout << std::hex << (int)arr[i] << "\t";
}

status_t compare_byte_arrays(const byte_arr_t &arr1, const byte_arr_t &arr2)
{
    // Code
    if (arr1.size() != arr2.size())
        return AES_FAILURE;
    
    for (size_t i = 0; i < arr1.size(); i++)
    {
        if (arr1[i] != arr2[i])
        {
            cerr << endl << "Error : Comparison Failed At Index : " << i << endl;
            return AES_FAILURE;
        }
    }

    return AES_SUCCESS;
}

status_t compare_vector_of_byte_arrays(const vector<byte_arr_t> &arr1, const vector<byte_arr_t> &arr2)
{
    // Variable Declarations
    status_t result;

    // Code
    if (arr1.size() != arr2.size())
        return AES_FAILURE;
    
    for (size_t i = 0; i < arr1.size(); i++)
    {
        if (arr1[i] != arr2[i])
            result = compare_byte_arrays(arr1[i], arr2[i]);
        if (result == AES_FAILURE)
        {
            cerr << endl << "Error : Comparison Failed At Index : " << i << endl;
            return AES_FAILURE;
        }
    }

    return AES_SUCCESS;
}

byte_arr_t XOR(const byte_arr_t &arr1, const byte_arr_t &arr2)
{
    // Code
    byte_arr_t result(arr1.size(), 0x00);

    for (size_t i = 0; i < arr1.size(); i++)
        result[i] = arr1[i] ^ arr2[i];
    
    return result;
}

void XOR(byte_arr_t &arr1, byte_arr_t &arr2, const unsigned int length)
{
    // Code
    for (int i = 0; i < length; i++)
        arr1[i] ^= arr2[i];
}

byte_arr_t increment_counter(const byte_arr_t &start_counter, const unsigned int round)
{
    // Variable Declarations
    int64_t counter_converted = 0x00;
    unsigned int counter_size = start_counter.size();
    byte_arr_t result(counter_size, 0x00);

    // Code
    for (unsigned int i = 0; i < counter_size; i++)
        counter_converted += (int64_t)(start_counter[start_counter.size() - i - 1]) << 8 * i   & (int64_t)0xFF << 8 * i;

    counter_converted += (uint64_t) round;

    for (unsigned int i = 0; i < counter_size; i++)
        result[i] += (counter_converted >> (counter_size - 1 - i) * 8) & (int64_t)0xFF;

    return result;
}

void generate_counters(vector<byte_arr_t> &counters, const byte_arr_t &iv)
{
    // Code
    byte_arr_t start_counter(AES_LENGTH - iv.size(), 0x00);
    byte_arr_t i_counter(AES_LENGTH - iv.size(), 0x00);
    byte_arr_t result(AES_LENGTH, 0x00);

    // Code
    for (size_t i = 0; i < counters.size(); i++)
    {
        result = iv;

        if (i > 0)
            i_counter = increment_counter(start_counter, i);
        
        result.insert(result.end(), i_counter.begin(), i_counter.end());
        counters[i] = result;
    }
}

byte_arr_t generate_iv(int iv_length)
{
    // Variable Declarations
    byte_arr_t byte_array(iv_length);
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 15);

    // Code
    for (size_t i = 0; i < byte_array.size(); i++)
        byte_array[i] = (byte_t) distribution(generator);

    return byte_array;
}

hash_t generate_hash(string input)
{
    // Variable Declarations
    long long k = 7;

    // Code
    for (int i = 0; i < input.length(); i++)
    {
        k *= 23;
        k += input[i];
        k *= 13;
        k %= 1000000009;
    }

    return k;
}

status_t set_key(string str_key, byte_t* aes_key, int *expanded_key_length)
{
    // Code
    if (str_key.length() <= AES_LENGTH)
    {
        int padding_length = AES_LENGTH - str_key.length();

        for (int i = 0; i < padding_length; i++)
            str_key.insert(str_key.length(), "0");
    }
    else
    {
        cerr << endl << "Key Length Should Not Be Greater Than 16 ... Exiting !!!" << endl;
        return AES_FAILURE;
    }
    
    memcpy(aes_key, str_key.c_str(), AES_LENGTH);

    *expanded_key_length = expand_key(aes_key);
}

status_t verify_key(string encryption_key, string decryption_key)
{
    // Code
    if (generate_hash(encryption_key) != generate_hash(decryption_key))
    {
        if (DEBUG)
            cerr << endl << "Invalid Key Entered ... Exiting !!!" << endl;
        return AES_FAILURE;
    }
}
