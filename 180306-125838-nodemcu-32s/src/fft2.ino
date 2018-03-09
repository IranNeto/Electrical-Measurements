double calculeTHF(double *magnitudeHarmonic){
	double sumMagnitudeHarmonic = 0;
	double fundamentalRMS = (sqrt(magnitudeHarmonic[1]/6)*voltsPerBit/sensibility)/sqrt(2);
	Serial.print("Fundamental: ");
	Serial.println(fundamentalRMS);

	for(int i = 2; i < 20; i++){
		magnitudeHarmonic[i] = sqrt(magnitudeHarmonic[i]/10)*voltsPerBit/sensibility; //calcula I
		magnitudeHarmonic[i] = magnitudeHarmonic[i]*magnitudeHarmonic[i]; //I*I
		sumMagnitudeHarmonic += magnitudeHarmonic[i]; //somatorio
	}
	Serial.println(sumMagnitudeHarmonic, 6);
	return((sqrt(sumMagnitudeHarmonic)/fundamentalRMS)*100);
}

double calculeTHD(double *magnitudeHarmonic){
	double sumMagnitudeHarmonic = 0;
	double fundamental = sqrt(magnitudeHarmonic[1]/10)*voltsPerBit/sensibility;
	Serial.print("Fundamental: ");
	Serial.println(fundamental);

	for(int i = 2; i < 20; i++){
		magnitudeHarmonic[i] = sqrt(magnitudeHarmonic[i]/10)*voltsPerBit/sensibility; //calcula I
		magnitudeHarmonic[i] = magnitudeHarmonic[i]*magnitudeHarmonic[i]; //I*I
		sumMagnitudeHarmonic += magnitudeHarmonic[i]; //somatorio
	}
	Serial.println(sumMagnitudeHarmonic, 6);
	return((sqrt(sumMagnitudeHarmonic)/fundamental)*100); 
}

void printHarmonicValues(double* magnitudeHarmonic){
	Serial.println("======================\n===========================\n======================");
	for(int i = 0; i < 20; i++){
		Serial.print("nHarmonic: ");
		Serial.print(i);
		Serial.print("\t,");
		Serial.print("Value: ");
		Serial.println(magnitudeHarmonic[i]);
	}
}

void calculaNamostras(double tempoAmostragem, int data[samples]){
	/* PEGAR DADOS PARA PLOTAR GRÁFICO DE EXEMPLO*/
	/*
	delay(2000);
	Serial.print("tempo de amostragem em ms = ");
	Serial.println(tempoAmostragem);
	Serial.print("Frequencia: ");
	Serial.println(500.0/(tempoAmostragem/1000));
	for(int i = 0; i < 500; i++){
		Serial.println(data[i]);
	}
	delay(1000000);
	*/
	
	samplingFrequency = (samples/(tempoAmostragem/1000));
	double cycles = (((samples-1) * signalFrequency) / samplingFrequency); //Number of signal cycles that the sampling will read
	for (uint16_t i = 0; i < samples; i++)
	{
		vReal[i] = data[i];
		//vReal[i] = int8_t((amplitude * (sin((i * (twoPi * cycles)) / samples))) / 2.0);/* Build data with positive and negative values*/
		//vReal[i] = uint8_t((amplitude * (sin((i * (twoPi * cycles)) / samples) + 1.0)) / 2.0);/* Build data displaced on the Y axis to include only positive values*/
		vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
	}
	
	/* Print the results of the simulated sampling according to time */
	//Serial.println("Data:");
	//PrintVector(vReal, samples, SCL_TIME);
	FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);	/* Weigh data */
	//Serial.println("Weighed data:");
	//PrintVector(vReal, samples, SCL_TIME);
	FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
	//Serial.println("Computed Real values:");
	//PrintVector(vReal, samples, SCL_INDEX);
	//Serial.println("Computed Imaginary values:");
	//PrintVector(vImag, samples, SCL_INDEX);
	FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
	Serial.println("Computed magnitudes:");
	PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);
	double x = FFT.MajorPeak(vReal, samples, samplingFrequency);
	Serial.println(x, 6);
	delay(20000000); /* Repeat after delay */
}

void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
	nHarmonic++;
	delay(2000);

	for (int i = 0; i < bufferSize; i++)
	{
		delay(20);
		double abscissa;
		/* Print abscissa value */
		switch (scaleType)
		{
			case SCL_INDEX:
			abscissa = (i * 1.0);
			break;
			case SCL_TIME:
			abscissa = ((i * 1.0) / samplingFrequency);
			break;
			case SCL_FREQUENCY:
			abscissa = ((i * 1.0 * samplingFrequency) / samples);
			break;
		}
		
		if(abscissa > (signalFrequency * nHarmonic) - 3 && abscissa < (signalFrequency * nHarmonic) + 3){
			magnitudeHarmonic[nHarmonic] += vData[i];
		} else if (abscissa > signalFrequency*nHarmonic + 5){
			nHarmonic++;
		}
		Serial.print(abscissa, 6);
		Serial.print(" ");
		Serial.print(vData[i], 4);
		Serial.println();
	}
	Serial.println();
	printHarmonicValues(magnitudeHarmonic);
	double thd = calculeTHD(magnitudeHarmonic);
	double thf = calculeTHF(magnitudeHarmonic);
	Serial.print(thd, 6);
	Serial.println(" \%");
	Serial.print(thf, 6);
	Serial.println(" \%");
}