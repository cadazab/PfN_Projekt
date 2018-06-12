

*Protein nGram(Protein protein, unsigned long n){
	unsigned long numberOfNGrams;
	Protein *nGrams;
	unsigned long i,j;
	unsigned long residuesSize = 0;
	unsigned long firstResidueSize;

	numberOfNGrams = protein->nr_residues - n + 1;

	nGrams = malloc(sizeof(Protein) * numberOfNGrams);



	for(i = 0; i< numberOfNGrams; i++){
		nGrams[i] = malloc(sizeof(Residue)*n)


		nGrams[i]->name = "nGram";
		nGrams[i]->nr_residues = n;

		nGrams[i]->cAlphas = malloc(sizeof(Atom)*n); 
		for(j = 0, j<n, j++){
			nGrams[i]->cAlphas[j]=protein->cAlphas[i+j];
		}

		for()
		firstResidueSize = sizeof(protein->residues[0]);
		for(j = 1,j<n,j++){
			residuesSize += firstResidueSize + sizeof(protein->residues[0])
		}
		


		//Xname
		// residues
		// atoms
		//XcAlphas
		//Xnr_residues
		// nr_Atoms
	}
	

}
