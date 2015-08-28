struct node* mp3player::next_artist(struct node* current){
    char ca[32] = {0,};
	char na[32] = {0,};
    for(int i=0; i<32; i++){
		if(current->data[i] == '-'){
			ca[i] = 0x00;
			break;
		}else{
			ca[i] = current->data[i];
		}
	}
	do{
		current = current->right;
		for(int j=0; j<32; j++){
		if(current->data[j] == '-'){
			na[j] = 0x00;
			break;
		}else{
			na[j] = current->data[j];
		}
	}while(strncmp(ca, na, std::min(i,j));
    char command[100] = {0,};
    strcat(command, "mpc load \"");
    strcat(command, get_data(current));
    strcat(command, "\"");
    std::system(&command[0]);
    //std::system(strcat("mpc load \"", next->PlayList));
    std::system("mpc play");
    return current;
}
struct node* mp3player::prev_artist(struct node* current){
    char ca[32] = {0,};
	char na[32] = {0,};
    for(int i=0; i<32; i++){
		if(current->data[i] == '-'){
			ca[i] = 0x00;
			break;
		}else{
			ca[i] = current->data[i];
		}
	}
	do{
		current = current->left;
		for(int j=0; j<32; j++){
		if(current->data[j] == '-'){
			na[j] = 0x00;
			break;
		}else{
			na[j] = current->data[j];
		}
	}while(strncmp(ca, na, std::min(i,j));
    char command[100] = {0,};
    strcat(command, "mpc load \"");
    strcat(command, get_data(current));
    strcat(command, "\"");
    std::system(&command[0]);
    //std::system(strcat("mpc load ", previous->PlayList));
    std::system("mpc play");
    return current;
}
