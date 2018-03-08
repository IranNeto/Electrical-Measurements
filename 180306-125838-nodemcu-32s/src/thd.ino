float* getTHD(float* freq, float I){
    float* normalize = (float*) malloc(10*sizeof(float));
    float base = freq[0];
    for(int i = 0; i < 10; i++){
        normalize[i] =
        normalize[i] = freq[i]/base;
        normalize[i] = I * normalize[i];
    }
    return normalize;
}