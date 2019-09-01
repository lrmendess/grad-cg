class Circle {
    private:
        float radius;
        float x;
        float y;

    public:
        Circle(float radius, float x, float y);
        
        void setRadius(float radius);
        void setX(float x);
        void setY(float y);

        float getRadius();
        float getX();
        float getY();
};