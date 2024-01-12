/*
 * PeakDetector.c
 *
 */
#include <TacheFFTClassification/PeakDetector.h>
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

void DetectPeak(int NbPeaks, float spectrum[], int spectrumSize, float Out[])
{
    //Copie du spectre dont les pics seront �limin�s
    float spectrumFiltered[spectrumSize];
    for (int i = 0; i < spectrumSize; i++)
    {
        spectrumFiltered[i] = spectrum[i];
    }

    //Init de la sortie
    for (int i = 0; i < NbPeaks; i++)
    {
        Out[2*i] = 0;
        Out[2*i+1] = 0;
    }

    float selectedPeakAmplitude = 0.0;
    int selectedPeakPosition = 0;
    int OutWriteIndex = 0;

    //On recherche jusqu'� 2n pics dans la FFT
    //Mais on s'arr�t quand on en a trouv� n non nuls
    for (int i = 0; i < 2*NbPeaks; i++)
    {
        //Si on a n peaks, on quitte
        if(OutWriteIndex>=NbPeaks)
            break;

        //On cherche le pic principal
        selectedPeakAmplitude = 0;
        selectedPeakPosition = 0;
        for (int j = 0; j < spectrumSize/2; j++)
        {
            if (spectrumFiltered[j] > selectedPeakAmplitude) //Search the max peak
            {
                selectedPeakAmplitude = spectrumFiltered[j];
                selectedPeakPosition = j;
            }
        }

        // On stocke le peak dans la s�rie de sortie
        // en �liminant un �ventuel pic en 0 correspondant � la composante continue
        if(selectedPeakPosition!=0)
        {
            Out[2 * OutWriteIndex] = selectedPeakPosition;
            Out[2 * OutWriteIndex + 1] = selectedPeakAmplitude;
            OutWriteIndex++;
        }

        //On �limine le pic
        //On �limine cot� freq croissantes
        int picPos = selectedPeakPosition;
        spectrumFiltered[picPos] = 0;
        while (picPos + 1 < spectrumSize/2)
        {
            if (spectrum[picPos + 1] < spectrum[picPos]) //Search the max peak
            {
                spectrumFiltered[picPos + 1] = 0;
                picPos++;
            }
            else break;
        }

        //On �limine cot� freq decroissantes
        picPos = selectedPeakPosition;
        while (picPos - 1 >= 0)
        {
            if (spectrum[picPos - 1] < spectrum[picPos]) //Search the max peak
            {
                spectrumFiltered[picPos - 1] = 0;
                picPos--;
            }
            else break;
        }
    }
}
