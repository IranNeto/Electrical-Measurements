float getOffset(){
    float sumOffsetData = 0;
    for(int i = 0; i < 20; i++){
        sumOffsetData += analogRead(A0);
        delay(2);
    }

    return(sumOffsetData/20.0); //average

}

float getTHD(float* freq, float I){
    float sumHarmonic = 0;
    float* normalize = (float*) malloc(nharmonics*sizeof(float));
    for(int i = 0; i < 10; i++){
        normalize[i] = 0;

        //The amplitude in each harmonic reduces N^-1 and N = harmonic order multiplied with
        //the relative proportion with the fundamental harmonic contribution
        normalize[i] = ( (1.0/(i+1.0)) * I * (freq[i]/freq[0])); 
        if(i != 0)
            sumHarmonic += normalize[i] * normalize[i]; //See the THD formula
    }
    
    return sqrt((sumHarmonic)/I)*100; //Percent
}