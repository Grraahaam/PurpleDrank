typedef struct MyVariables {
    const int screenWidth = 800;
    const int screenHeight = 450;
    float vitesse = VELOCITY*0.4;
    bool boule = true;
    bool right = true;
    bool col_trou = false;
    bool col_wall_right = false;
    bool victory = false;
    int fall = 0;
    int nb_lives = 5;
};
extern struc MyVariables my_Var;

