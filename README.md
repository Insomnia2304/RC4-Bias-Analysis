# RC4_Bias_Analysis

/* 
 * 1 / 256 = 0.00390625
 *
 * BIAS:
 * P(output_byte2 = 0) ~= 1 / 128 = 0.0078125
 * P(output_byte3 = 0) ~= 0.003911607
 * etc.
 * 
 */

After running the algorithm you will observe that starting from the 2nd byte there is a much higher probability of obtaining a value of 0.
The param ```number_of_tests``` should be set as high as possible for clearer results.
