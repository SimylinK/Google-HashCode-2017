#pragma once

//Fonction pour cr�er l'objet Plan � partir d'un fichier en entr�e
template <unsigned int H, unsigned int W>
Plan<H, W> parser(char *file);