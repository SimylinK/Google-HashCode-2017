#pragma once

//Fonction qui doit remplir le fichier d'ouput à partir d'une instance de Plan
template <unsigned int H, unsigned int W>
void output(Plan<H, W> plan, char* outputFile);