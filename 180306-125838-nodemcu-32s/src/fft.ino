numero soma(numero a, numero b){
	numero res;
	res.real = a.real + b.real;
	res.imag = a.imag + b.imag;	
	return res;
}

numero atribuir(float real, float imag){
	numero res;
	res.real = real;
	res.imag = imag;
	return res;
}

numero sub(numero a, numero b){
	numero aux = {-1*b.real, -1*b.imag};
	return soma(a,aux);
}

numero mult(numero a, numero b){
	numero res;
	res.real = a.real * b.real - a.imag * b.imag;
	res.imag = a.real * b.imag + b.real * a.imag;
	return res;
}

void printNumero(numero a){
	Serial.print(a.real);
	Serial.print(" ");
	Serial.println(a.imag);
}
void printNormalHarmonics(float* harmonic){
	float hPercent[8] = {};
	for(int i = 0; i < 8; i++){
		Serial.println(100*harmonic[i]/harmonic[0]);
	}

}

void printHarmonicsContribuition(float* harmonic){
	Serial.println("Harmonicos: ");
	for(int i = 0; i < 8; i++){
		Serial.print(i);
		Serial.print(" - ");
		Serial.println(harmonic[i]);
	}

	Serial.println("=================");
	printNormalHarmonics(harmonic);
}

void printNumeros(numero *a, int N, bool withMag){

	float harmonics[8] = {};
	int n = 0;
	for(int i=0; i < N; i++){
		if(withMag){
			//Serial.print(i);
			//Serial.print(" - ");
			Serial.println(sqrt(a[i].real * a[i].real + a[i].imag * a[i].imag), 6);
			delay(10);
		} else if(!(i%60) && i >= 60){
			for(int j = -5; j < 5; j++){
				harmonics[n] += sqrt(a[i+j].real * a[i+j].real + a[i+j].imag * a[i+j].imag);
			}
			n++;
		}
	}
	Serial.println("===============================");
	if(!withMag){
		printHarmonicsContribuition(harmonics);
	}
	
}

numero* radix2(numero a, numero b){
	struct numero* res = (struct numero*) malloc(2 * sizeof(struct numero));
	res[0] = soma(a,b);
	res[1] = sub(a,b);
	return res;
}

numero* pares(numero *x, int N){
	struct numero* res = (struct numero*) malloc((N/2) * sizeof(struct numero));
	int cont = 0;
	for(int i=0; i<N; i+=2){
		res[cont++] = x[i];
	}
	return res;
}

numero* impares(numero *x, int N){
	struct numero* res = (struct numero*) malloc((N/2) * sizeof(struct numero));
	int cont = 0;
	for(int i=1; i<N; i+=2){
		res[cont++] = x[i];
	}
	return res;
}

numero Wn(int k, int N){
	float n = (float) N;
	numero W = {cos(2*PI*k/n), -1*sin(2*PI*k/N)};
	return W;
}

numero* radix(numero *x, int N){
	struct numero* res = (struct numero*) malloc((N) * sizeof(struct numero));
	if(N == 2){
		struct numero* res = (struct numero*) malloc(2 * sizeof(struct numero));
		res[0] = soma(x[0],x[1]);
		res[1] = sub(x[0],x[1]);
		return res;
	}else{
		numero *P = radix( pares(x, N) , N/2);
		numero *I = radix( impares(x, N) , N/2);
		for(int k=0; k<N; k++){
			res[k] = soma(P[k%(N/2)], mult(Wn(k,N),I[k%(N/2)]));
		}
		return res;
	}
}

int flipBits(int n, int k){
	int mask = 1;
	int res = 0;
	int p = k/2;
	res = p*(n & mask);
	for(int i=0; i<(k/2); i++){
		n = (n >> mask);
		p = p/2;
		res += p*(n & mask);
	}
	return res;
}


numero* ordenar(numero* ordFFT, int N){
	struct numero* ordCresc = (struct numero*) malloc((N) * sizeof(struct numero));
	for(int i=0; i<N; i++){
		ordCresc[i] = ordFFT[flipBits(i, N)];
		//printf("ordCresc[%d] = ordFFT[%d]\n", i, flipBits(i, N));
	}
	return ordCresc;
	
}

void getSpectrum(int tempoAmostragem, int* data){	
	int N = 512;
	struct numero* input = (struct numero*) malloc((N) * sizeof(struct numero));
	int cont = 0;
	float value;
	for(int i=0; i<N; i++){
		value = cos(2*i*3.14*60);
		input[i] = atribuir(value, 0);
		Serial.println(value, 6);
	}
	Serial.println("=================================");
	numero *ordFFT = radix(input,N);
	numero *ordCresc = ordenar(ordFFT, N);
	printNumeros(ordCresc, N, true);
	printNumeros(ordCresc, N, false);
}