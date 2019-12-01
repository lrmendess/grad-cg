#include <GL/glut.h>
#include <stdio.h>

GLuint LoadTexture( const char * filename ) {
    GLuint texture;

    int width, height;

    unsigned char * data;

    FILE * file;

    file = fopen( filename, "rb" );

    if ( file == NULL ) return 0;
    width = 1024;
    height = 512;
    data = (unsigned char *)malloc( width * height * 3 );
    //int size = fseek(file,);
    fread( data, width * height * 3, 1, file );
    fclose( file );

    for(int i = 0; i < width * height ; ++i) {
        int index = i*3;
        unsigned char B,R;
        B = data[index];
        R = data[index+2];

        data[index] = R;
        data[index+2] = B;
    }


    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D,                            // Always GL_TEXTURE_2D
                             0,                            // 0 for now
                             GL_RGB,                       // Format OpenGL uses for image
                             width, height,                // Width and height
                             0,                            // The border of the image
                             GL_RGB,                       // GL_RGB, because pixels are stored in RGB format
                             GL_UNSIGNED_BYTE,             // GL_UNSIGNED_BYTE, because pixels are stored
                                                           // as unsigned numbers
                             image->pixels);               // The actual pixel data

    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
    free( data );

    return texture;
}
