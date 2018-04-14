#include "fastfouriertransform.h"

FastFourierTransform::FastFourierTransform(int inSampleSize)
{
    this->sampleSize = inSampleSize;
    tempImage = new QVector<std::complex<double>>(inSampleSize);
}

FastFourierTransform::~FastFourierTransform()
{
    delete tempImage;
}

QVector<std::complex<double>>* FastFourierTransform::CombineResults(int n, QVector<std::complex<double>>* b, QVector<std::complex<double>>* c)
{
    tempImage = new QVector<std::complex<double>>(sampleSize);
    for (int  i = 0; i < n / 2; i++)
    {
        (*tempImage)[2 * i] = (*b)[i];
        (*tempImage)[2 * i + 1] = (*c)[i];
    }
    return tempImage;
}

QVector<std::complex<double>>* FastFourierTransform::BPF(QVector<std::complex<double>>* a, bool minus){
    int n = (*a).size();
    if (n == 1) return a;

    QVector<std::complex<double>> c(n/2), b(n/2);
    std::complex<double> w = {1, 0};
    for(int j = 0; j < n/2; j++){
        b[j] = (*a)[j] + (*a)[j + n / 2];
        c[j] = ((*a)[j] - (*a)[j + n / 2]) * w;
        w *= std::complex<double> {cos(minus?2*M_PI/n:-2*M_PI/n), sin(minus?2*M_PI/n:-2*M_PI/n)};
    }
    return CombineResults(n, BPF(&b, minus), BPF(&c, minus));
}

void FastFourierTransform::Draw(QVector<double>* y)
{
    QVector<double> X_f(sampleSize);
    QVector<double> Y_f(sampleSize);
    QVector<std::complex<double>> F(sampleSize);

    for(int i = 0; i < sampleSize; i++)
    {
        F[i] = std::complex<double>{(*y)[i], 0};//myEquationComplex(i*2*M_PI/(N-1));
    }
    F = *BPF(&F);
    for(int i = 0; i < sampleSize; i++)
    {
        X_f[i] = i;
        F[i] = std::complex<double> {F[i].real()/sampleSize, F[i].imag()/sampleSize};
        (*y)[i] = sqrt(pow(F[i].real(), 2) + pow(F[i].imag(), 2));
    }
}

