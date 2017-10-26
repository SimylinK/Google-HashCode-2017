#pragma once

//Fonction pour créer l'objet Plan à partir d'un fichier en entrée
template <unsigned int H, unsigned int W>
Plan<H, W> parser(char *file);