#include <iostream>
#include <fstream>
using namespace std;

float getLength(int eumpyo) {
	if (eumpyo == 4) return 1.0;
	else if (eumpyo == 8) return 0.5; // 8분음표는 0.5초
	else if (eumpyo == 16) return 0.25; // 16분음표는 0.25초
	else return -1;

}
float getAmplitude(int amplitude) {
	float a;
	a = amplitude * 1000;
	return a;

}
float getfrequency(char a) {
	if (a == 'C') return 523.0;
	else if (a == 'D') return 587.33;
	else if (a == 'E') return 659.62;
	else if (a == 'F') return 698.0;
	else if (a == 'G') return 392.0;
	else if (a == 'A') return 440.0;
	else if (a == 'B') return 493.88;
	else return 111;

}
float Note(float* f, float* a, float* T, short* akbo, int n) {
	int fs = 44100;
	const float pi = 3.141592;
	float dt = 1. / fs;
	for (int i2 = 0; i2 < T[n] * fs; i2++) {
		akbo[i2] = (short)(a[n] * sin(2.0 * pi * f[n] * i2 * dt));
	}
	return 1;
}
int main() {
	ifstream xx("Beatles-LetItBe-wav_30s.wav", ios::binary | ios::in);
	if (!xx) return 999;  // if the file is not found
	char header[44];


	// Sprint 1
	xx.read(header, 44);
	xx.close();
	int* sampleRate = (int*)(header + 24);
	cout << "   sampleRate = " << *sampleRate << endl;

	short* n;   n = (short*)(header + 22);  // numChannels
	int* fs;   fs = (int*)(header + 24);  // sampleRate
	int* b;   b = (int*)(header + 28);  //  byteRate

	*n = 1;  // mono
	*b = n[0] * fs[0] * sizeof(short);


	// Sprint 3
	int N = fs[0]; // number of samples for 1 second
	short* data;
	data = new short[N]; // allocate memory for 1 seconds mono
	const float pi = 3.141592;
	float dt = 1. / fs[0];
	int i = 0;

	float* a = 0;
	float* T = 0;
	float* f = 0;

	int note, eu, ss;
	char hh;
	short* akbo;

	akbo = new short[5 * fs[0]];

	ifstream zz("할아버지의시계.txt");
	zz >> note;
	f = new float[note];
	a = new float[note];
	T = new float[note];

	for (int i = 0; i < note; i++) {
		zz >> eu >> hh >> ss;  // eu ss는 int이고, hh는 char
		a[i] = getAmplitude(ss);
		f[i] = getfrequency(hh);
		T[i] = getLength(eu);

	}
	zz.close();

	ofstream yy("clock.wav", ios::binary | ios::out);
	if (!yy) return 656;
	yy.write(header, 44 * sizeof(char));

	for (int i = 0; i < note; i++) {
		Note(f, a, T, akbo, i);
		yy.write((char*)akbo, T[i] * fs[0] * 2);

	}
	yy.close();
	return 1;
}