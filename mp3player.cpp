#include "mp3player.h"
#include "ui_mp3player.h"

mp3player::mp3player(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mp3player)

{
    ui->setupUi(this);
    ui->centralWidget->setCursor(Qt::BlankCursor);
}

mp3player::~mp3player()
{
    delete ui;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Listen Funktionen (für die Playlisten)
struct node * mp3player::new_node_list(char entry[64]){
    struct node *new_node = (struct node*) malloc(sizeof(struct node));
    strcpy(new_node->data, entry);//  = -1;
    new_node->right = new_node;
    new_node->left  = new_node;
    return new_node;
}


struct node * mp3player::insert_right(struct node *list, char data[64]){
    struct node *new_node = (struct node *) malloc(sizeof(struct node));
    strcpy(new_node->data, data);
    new_node->left        = list;
    new_node->right       = list->right;
    list->right      = new_node;
    new_node->right->left = new_node;
    return new_node;
}

struct node * mp3player::insert_left(struct node *list, char data[64]){
    struct node *new_node = (struct node *) malloc(sizeof(struct node));
    strcpy(new_node->data, data);
    new_node->right        = list;
    new_node->left       = list->left;
    list->left      = new_node;
    new_node->left->right = new_node;
    return new_node;
}

struct node * mp3player::delete_node(struct node *list){
    list->right->left = list->left;
    list->left->right = list->right;
    return list->left;
}

void mp3player::print_all(struct node* list){
    struct node *head = list;
    struct node *current = list;
    printf("Node:%s \n\n", head->data);
    while (head != (current = current->right)){
        printf("Node:%s \n", current->data);
    }
    printf("\n");
}
struct node * mp3player::right_node(struct node* list){
    /*
    struct node *head = list;
    struct node *current = list;
    if(head != (current = current->right)){
        return current;
    }
    return head;
    //*/
    return list->right;
}
struct node * mp3player::left_node(struct node* list){
    /*
    struct node *head = list;
    struct node *current = list;
    if(head != (current = current->left)){
        return current;
    }
    return head;
    //*/
    return list->left;
}

char* mp3player::get_data(struct node* entry){
    return entry->data;
}

struct node * mp3player::restore(struct node *list){
    list->right->left = list;
    list->left->right = list;
    return list;
}

struct node * mp3player::delete_and_free(struct node *list){
    list->right->left = list->left;
    list->left->right = list->right;
    struct node *res  = list->left;
    free(list);
    return res;
}

//*/

struct node* mp3player::load_playlists(void){
    //return NULL;
    DIR           *d;
    struct dirent *dir;
    int i = 0;
    struct node* current;
    struct node* head = NULL;
    d = opendir(MUSICDIR);
    if (d){
        while ((dir = readdir(d)) != NULL){
            //sortieren!!!!
            printf("Folder: %s\n", dir->d_name);
            //*
            if(dir->d_name[0] != '.'){
                //int x;
                //*
                std::system("mpc clear");
                char command[100] = {0,};
                strcat(command, "mpc ls \"");
                strcat(command, dir->d_name);
                strcat(command, "\" | mpc add");
                std::system(&command[0]);
                //for(x=0;x<32; x++){command[x] = 0x00;}
                char command2[100] = {0,};
                strcat(command2, "mpc save \"");
                strcat(command2, dir->d_name);
                strcat(command2, "\"");
                std::system(command2);
                std::system("mpc clear");
                //*/
                //*
                if(i==0){
                    head = new_node_list(dir->d_name);
                    current = head;
                }else{
                    current = insert_sorted(current, dir->d_name);
                }
                i++;
                //*/
            }//*/

        }
        print_all(head);
        closedir(d);
    }
    return head;
}
/*
struct node* randomEntry(struct node* Head){
    int i;
    time_t t;
    int numberOfEntrys = 0;
    int entryNumber = 0;
    struct node* Position = Head;
    time(&t);
    if(Head->Next == Head) return NULL;
    while(Position->Next != Head){
        Position = Position->Next;
        numberOfEntrys++;
    }
    srand((unsigned int) t);
    entryNumber = (int) (rand() % numberOfEntrys);
    for(i=0;i<entryNumber; i++){
        Position = Position->Next;
    }
    return Position;
}
*/

void mp3player::swap_nodes(struct node* a, struct node* b){
    char temp[64] = {0,};
    strcpy(temp, a->data);
    strcpy(a->data, b->data);
    strcpy(b->data, temp);
}

struct node* mp3player::sort(struct node* list){
    struct node* i;
    struct node* j;
    for(i=list; i->right!=list; i=i->right){
        for(j=list; j->right!=list; j=j->right){
            if(strcmp(j->data, j->right->data) > 0){
                swap_nodes(j, j->right);
            }
        }
    }
    return list;
}


struct node* mp3player::insert_sorted(struct node* list, char data[64]){
    return insert_right(list, data);
}

struct node* mp3player::next_playlist(struct node* current){
    struct node* next = right_node(current);
    char command[100] = {0,};
    strcat(command, "mpc load \"");
    strcat(command, get_data(next));
    strcat(command, "\"");
    std::system(&command[0]);
    //std::system(strcat("mpc load \"", next->PlayList));
    std::system("mpc play");
    return next;
}

struct node* mp3player::get_entry(struct node* head, char data[64]){
    struct node* current = head;
    while(strcmp(current->data, data) != 0){
        current = current->right;
        if(current == head){
            return NULL; //nichts gefunden!
        }
    }
    return current; //gefunden!
}

struct node* mp3player::prev_playlist(struct node* current){
    struct node* previous = left_node(current);
    char command[100] = {0,};
    strcat(command, "mpc load \"");
    strcat(command, get_data(previous));
    strcat(command, "\"");
    std::system(&command[0]);
    //std::system(strcat("mpc load ", previous->PlayList));
    std::system("mpc play");
    return previous;

}








struct node* mp3player::next_artist(struct node* current){
    char ca[32] = {0,};
    char na[32] = {0,};
    int i,j;
    for(i=0; i<32; i++){
        if(current->data[i] == '-'){
            ca[i] = 0x00;
            break;
        }else{
            ca[i] = current->data[i];
        }
    }
    do {
        current = current->right;
        for(j=0; j<32; j++){
            if(current->data[j] == '-'){
                na[j] = 0x00;
                break;
            }else{
                na[j] = current->data[j];
            }
        }
        printf("%s ca:%s na:%s strcmp: %i\n", get_data(current), ca, na, strncmp(ca, na, std::min(i,j)));
    } while (strncmp(ca, na, std::min(i,j))==0);
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
    int i,j;
    for(i=0; i<32; i++){
        if(current->data[i] == '-'){
            ca[i] = 0x00;
            break;
        }else{
            ca[i] = current->data[i];
        }
    }
    do{
        current = current->left;

        for(j=0; j<32; j++){
            if(current->data[j] == '-'){
                na[j] = 0x00;
                break;
            }else{
                na[j] = current->data[j];
            }
        }
        printf("%s ca:%s na:%s strcmp: %i\n", get_data(current), ca, na, strncmp(ca, na, std::min(i,j)));
    }while(strncmp(ca, na, std::min(i,j))==0);
    char command[100] = {0,};
    strcat(command, "mpc load \"");
    strcat(command, get_data(current));
    strcat(command, "\"");
    std::system(&command[0]);
    //std::system(strcat("mpc load ", previous->PlayList));
    std::system("mpc play");
    return current;
}



struct node* mp3player::check_playlists(void){
    FILE *fp;
    char playlist[64];
    struct node* head = NULL;
    struct node* current;
    int i = 0;//, c = 0;
    // Open the command for reading.
    fp = popen("mpc lsplaylists", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        return(NULL);
    }
    // Read the output a line at a time - output it.
    while (fgets(playlist, sizeof(playlist)-1, fp) != NULL) {
        strtok(playlist, "\n");
        if(i){
            current = insert_sorted(current, playlist);
        }else{
            head = new_node_list(playlist);
            current = head;
        }
        //printf("%s\n", playlist);
        i++;
    }
    // close
    pclose(fp);

    return head;
}

struct node* mp3player::delete_playlist(struct node* list){
    //QString playlist = str(this.get_data(list));
    //std::system("mpc rm " + playlist );
    char command[100] = {0,};
    strcat(command, "mpc rm \"");
    strcat(command, get_data(list));
    strcat(command, "\"");
    std::system(&command[0]);
    printf("%s\n", command);
    return delete_and_free(list);

}

void mp3player::delete_all_playlists(struct node* list){
    //struct node *head = list;
    struct node *current = list;
    while (current->right != current){
        printf("Delete: %s\n", current->data);
        current = delete_playlist(current);
        print_all(current);
    }
    char command[100] = {0,};
    strcat(command, "mpc rm \"");
    strcat(command, get_data(current));
    strcat(command, "\"");
    std::system(&command[0]);
    std::system("mpc update --wait");
    strcpy(current->data, "");
    PlayListHead = current;
    CurrentPlayList = current;
}

struct node* mp3player::get_last_playlist(void){
    FILE *fp;
    struct node* saved_list = NULL;
    char text[64];
    fp = fopen("last_playlist","r");
    if (fp == NULL) {
        printf("Failed to open file\n" );
        return PlayListHead;
    }
    if(fgets(text, sizeof(text)-1, fp) != NULL) {
        //strtok(text, "\n");
        //printf("%s\n", text);
        saved_list = get_entry(PlayListHead, text);
        //printf("%s\n",get_data(saved_list));
        if(saved_list == NULL) saved_list = PlayListHead;
    }
    fclose(fp);
    return saved_list;
}

void mp3player::save_playlist(struct node* list){
    FILE *fp;
    fp = fopen("last_playlist","w");
    if (fp == NULL) {
        printf("Failed to open file\n" );
        return;
    }
    fprintf(fp, "%s", get_data(list));
    fclose(fp);
}

void mp3player::print_song(void){
    FILE *fp;
    int l = 0;
    char text[64];

    // Open the command for reading.
    fp = popen("mpc -f '%artist% - %album%\n%title%'", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        return;
    }
    //if(status == status_mp3player){
        ui->stackedWidget->setCurrentIndex(0);
    //}else if(status == status_i2c){
    //    ui->stackedWidget->setCurrentIndex(1);
    //}else if(status == status_navit){
    //    ui->stackedWidget->setCurrentIndex(2);
    //}
	// das playlistlabel hält bei schriftgröße 12f 29Zeichen, 10f 37 Zeichen
/*    if(strlen(get_data(CurrentPlayList)) < 28){
        QFont font = ui->mp3_label_playlist_2->font();
        font.setPointSize(2*12);
		ui->mp3_label_playlist_2->setFont(font);
    }else if(strlen(get_data(CurrentPlayList)) <38) {
        QFont font = ui->mp3_label_playlist_2->font();
        font.setPointSize(2*10);
		ui->mp3_label_playlist_2->setFont(font);
    }else if(strlen(get_data(CurrentPlayList)) < 45){
        QFont font = ui->mp3_label_playlist_2->font();
        font.setPointSize(2*8);
        ui->mp3_label_playlist_2->setFont(font);
    }else{
        QFont font = ui->mp3_label_playlist_2->font();
        font.setPointSize(2*7);
        ui->mp3_label_playlist_2->setFont(font);
    }
*/
    // Read the output a line at a time - output it.
    while (fgets(text, sizeof(text)-1, fp) != NULL) {
        strtok(text, "\n");
        QString QText = QString::fromUtf8(text);// = new QString(text);
        if(QText.contains("repeat: on")){
            ui->label3->setText("Repeat on");
        }else if(QText.contains("repeat: off")){
            ui->label3->setText("Repeat off");
        }
        if(QText.contains("random: on")){
            ui->mp3_label_playlist_2->setText(get_data(CurrentPlayList));
            QString text = ui->mp3_label_playlist_2->text();
            text.append(" R");
            if(backcount > 0){
                text.append("*");
            }
            ui->mp3_label_playlist_2->setText(text);
        }else if(QText.contains("random: off")){
            ui->mp3_label_playlist_2->setText(get_data(CurrentPlayList));
            QString text = ui->mp3_label_playlist_2->text();
            if(backcount > 0){
                text.append("*");
            }
            ui->mp3_label_playlist_2->setText(text);
        }
        if(QText.contains("single: on")){
            ui->label6->setText("Single on");
        }else if(QText.contains("single: off")){
            ui->label6->setText("Single off");
        }
        if(l==0){
            if(QText.contains("volume:") && (!(ui->mp3_label_title_2->text().contains("volume:")))){
                // playlist ist zu ende.. die naechste bitte ;)
                std::system("mpc clear");
                CurrentPlayList = next_playlist(CurrentPlayList);
                return;
            }
	    if(QText.length() < 28){
	        QFont font = ui->mp3_label_playlist_2->font();
            font.setPointSize(2*12);
	        ui->mp3_label_playlist_2->setFont(font);
	    }else if(QText.length() <38) {
	        QFont font = ui->mp3_label_playlist_2->font();
            font.setPointSize(2*10);
	                ui->mp3_label_playlist_2->setFont(font);
	    }else if(QText.length() < 45){
	        QFont font = ui->mp3_label_playlist_2->font();
            font.setPointSize(2*8);
	        ui->mp3_label_playlist_2->setFont(font);
	    }else{
	        QFont font = ui->mp3_label_playlist_2->font();
            font.setPointSize(2*7);
	        ui->mp3_label_playlist_2->setFont(font);
	    }

/*		if(QText.length() < 25){
			QFont font = ui->mp3_label_playlist_2->font();
            font.setPointSize(2*14);
			ui->mp3_label_playlist_2->setFont(font);
		}else if(QText.length() < 52){
                    QFont font = ui->mp3_label_playlist_2->font();
                    font.setPointSize(2*13);
                    ui->mp3_label_playlist_2->setFont(font);
                }else{
                    QFont font = ui->mp3_label_playlist_2->font();
                    font.setPointSize(2*12);
                    ui->mp3_label_playlist_2->setFont(font);
                }
*/		ui->mp3_label_playlist_2->setText(QText);
	}else if(l==1){

            //if(status == status_mp3player){
                ui->top_label->setText("Navigation inaktiv");
				
                if(QText.length() < 25){
                    QFont font = ui->mp3_label_title_2->font();
                    font.setPointSize(2*14);
					ui->mp3_label_title_2->setFont(font);
                }else if(QText.length() < 52){
                    QFont font = ui->mp3_label_title_2->font();
                    font.setPointSize(2*13);
					ui->mp3_label_title_2->setFont(font);
				}else{
                    QFont font = ui->mp3_label_title_2->font();
                    font.setPointSize(2*12);
					ui->mp3_label_title_2->setFont(font);
				}
				
				
               ui->mp3_label_title_2->setText(QText);
            /*}else{
                if(QText.length() < 31){
                    QFont font = ui->top_label->font();
                    font.setPointSize(2*12);
					ui->top_label->setFont(font);
                }else if(QText.length() < 46){
                    QFont font = ui->top_label->font();
                    font.setPointSize(2*10);
					ui->top_label->setFont(font);
				}else{
                    QFont font = ui->top_label->font();
                    font.setPointSize(2*9);
					ui->top_label->setFont(font);
				}
                ui->top_label->setText(QText);
                //ui->mp3_label_title_2->setText(QText);
            }
            // */
            //printf("1: %s\n", text);
            //put first line to label
        }else if(QText.contains("[p")){
            ui->mp3_misc_2->setText(QText);
            //printf("2: %s\n\n\n\n\n\n\n\n\n\n\n\n", text);
            //put line 2 to label ;)
        }

        //printf("X: %s\n", text);
        l++;
    }

    // close
    pclose(fp);
}
int mp3player::mp3init(void){


    if (wiringPiSetupGpio () == -1){
        printf("WiringPi setup failed!");
        return 1 ;
    }
    status = status_mp3player;
    pinMode(READY_PIN, OUTPUT);
    //pinMode(ACTIVE_PIN, OUTPUT);
    //digitalWrite(ACTIVE_PIN, HIGH);
    digitalWrite(READY_PIN, LOW);
    pinMode(SHUTDOWN_PIN, INPUT);
    sleep(1);
    PlayListHead = check_playlists();
    if(PlayListHead == NULL){ // mpc kennt keine playlists
        printf("Playlists:");
        PlayListHead = load_playlists();
        char command[100] = {0,};
        std::system("mpc clear");
        strcat(command, "mpc load \"");
        strcat(command,  get_data(PlayListHead));
        strcat(command, "\"");
        std::system(command);
        sort(PlayListHead);
        CurrentPlayList = PlayListHead;
        //printf("Playlists loaded!\n");
    }else{ // mpc kennt bereits playlists
        sort(PlayListHead);
        CurrentPlayList = get_last_playlist();
        //letzte playlist laden
    }
    backcount = 0;
    std::system("mpc play");


    ser = serialOpen ("/dev/ttyAMA0", 38400);
    if(ser == -1){
        printf("error opening /dev/ttyAMA0!\nexiting....");
        return -1;
    }
    QTimer *timer_50ms = new QTimer(this);
    connect(timer_50ms, SIGNAL(timeout()),this,SLOT(mp3_task()));
    timer_50ms->start(50);
    QTimer *timer_200ms = new QTimer(this);
    connect(timer_200ms, SIGNAL(timeout()),this,SLOT(print_song()));
    timer_200ms->start(200);
    //start_navit();
    return 0;
}

void mp3player::mp3_task(){
    if(serialDataAvail(ser)>0){
        unsigned char rx = serialGetchar(ser);

        if(rx != '0'){
            printf("Taste: %c\n",rx);
        }
        if(rx == 0xFF){// radio ausgeschaltet?
            ff_count++;
            printf("FF_count: %i\n",ff_count);
        }else{
            ff_count = 0;

            if(rx != rx_old){


                if(rx == 0x01){//rpimfdinterface startete deb pi nach zv auf, radio ist aber noch nicht eingeschaltet - wir warten.
                    std::system("mpc pause");
                    sleep(1);
                }
                if(rx == '0' && rx_old == 0x01){// jetzt ist das radio an - los
                    std::system("mpc play");
                }
                if(rx == 0x02){//rpimfdinterface startete deb pi nach zv auf, radio ist aber noch nicht eingeschaltet - wir warten.
                    std::system("mpc pause");
                    sleep(1);
                }else if(rx == '0' && rx_old == 0x02){// jetzt ist das radio an - los
                    std::system("mpc play");
                }


                switch(rx){
                    // *
                    case 0x01:{//String zum tacho - mp3player inaktiv
                        for(int i = 0;i<17;i++){
                            rx = serialGetchar(ser);
                            if(rx==0){
                                printf("%s\n",radio_string.c_str());
                                radio_string.clear();
                            }else{
                                radio_string.push_back(rx);
                            }

                        }
                        rx = 0x01;
                        serialFlush(ser);
                        //return;
                        break;
                    }
                    // */
                    case USBenter:{
			if(backcount == 0){

                            std::system("mpc toggle");
			}

                        break;
                    }

                    case USBnext:{
                        std::system("mpc next");
                        break;
                    }
                    case USBprev:{
                        std::system("mpc prev");
                        break;
                    }
                    case USB_1:{//prev album
                        std::system("mpc clear");
                        CurrentPlayList = prev_playlist(CurrentPlayList);
                        break;
                    }
                    case USB_2:{//next album
                        std::system("mpc clear");
                        CurrentPlayList = next_playlist(CurrentPlayList);
                        break;
                    }
                    case USB_3:{
                        std::system("mpc repeat");
                        break;
                    }
                    case USB_4:{
                        std::system("mpc clear");
                        CurrentPlayList = prev_artist(CurrentPlayList);
                        break;
                    }
                    case USB_5:{
                        std::system("mpc clear");
                        CurrentPlayList = next_artist(CurrentPlayList);
                        break;
                    }
                    case USB_6:{
                        std::system("mpc single");
                        break;
                    }
                    case USBscan:{
                        delete_all_playlists(PlayListHead);
                        //PlayListHead = load_playlists();
                        std::system("mpc stop");
                        std::system("mpc update");

                        PlayListHead = load_playlists();
                        sort(PlayListHead);
                        CurrentPlayList = PlayListHead;
                        std::system("mpc update");

                        break;
                    }
                    case USBas:{//cd mix
                        std::system("mpc random");
                        break;
                    }
                    case USBaudio:{
                        if(backcount>0){
                            backcount = 0;
                        }else{
                            backcount = 3;
                        }
                        break;
                    }
                    case USBtone:{
                        if(backcount>0){
                            backcount = 0;
                        }else{
                            backcount = 3;
                        }
                        break;
                    }
                    case USBinfo:{
                    /*
                        if(status != status_i2c){
                            printf("\nI2C %i\n", status);
                            status = status_i2c;
                        }else{
                            printf("\nMP3PLAYER %i\n", status);
                            status = status_mp3player;
                        }
                        */
                        break;
                    }
                    case USBback:{

                        if(backcount>0) backcount--;

                        break;
                    }
                    case USBeject:{
                        CurrentPlayList = delete_playlist(CurrentPlayList);
                        std::system("mpc clear");
                        // *
                        CurrentPlayList = next_playlist(CurrentPlayList);
                        std::system("mpc update");

                        break;
                    }
                    case USBflag:{

                        delete_all_playlists(PlayListHead);
                        //PlayListHead = load_playlists();
                        std::system("mpc stop");
                        std::system("mpc update");
                        //navit start navigation ? get town
                        break;
                    }
                    case USBnavi:{
                       /*
                        if(status != status_navit){
                            printf("\nNAVIT %i\n", status);
                            status = status_navit;
                            printf("\nNAVIT %i\n", status);
                        }else{
                            printf("\nMP3PLAYER %i\n", status);
                            status = status_mp3player;
                             printf("\nMP3PLAYER %i\n", status);
                        }
                        //*/
                        break;
                    }
                    case USBtraffic:{
                        PlayListHead = load_playlists();
                        CurrentPlayList = PlayListHead;
                        sort(PlayListHead);
                        std::system("mpc update");
                        // navit stop navigation
                        break;
                    }
                    case USBtim:{
                        //std::system("mpc toggle");
                        break;
                    }
                    case USBplus:{
                        std::system("mpc volume +1");
                        break;
                    }
                    case USBminus:{
                        std::system("mpc volume -1");
                        break;
                    }
                    default:{
                        serialFlush(ser);
                        break;
                    }
                }
            }
            serialFlush(ser);
            rx_old = rx;
        }
        if(ff_count > 10){
            save_playlist(CurrentPlayList);
            system("mpc pause");
            //digitalWrite(ACTIVE_PIN, LOW);
            digitalWrite(READY_PIN, HIGH);
            status = status_off;
            printf("Status: OFF!\n");
            return;
        }

    }
}

void mp3player::start_navit(void){
    QRect geo = ui->x11embedContainer->geometry();
    ui->x11embedContainer = new QX11EmbedContainer(ui->navit);
    ui->x11embedContainer->setGeometry(geo);
    QString wId = QString::number(ui->x11embedContainer->winId());
    setenv("NAVIT_XID", wId.toAscii(),1);
    navit = new QProcess(ui->x11embedContainer);
    ui->x11embedContainer->setFocus();
    navit->start("navit");
}



