int area_circle(int radius);

int volume_cylinder(int diameter, int height)
{
  int radius;
  int area;
  int volume;
  int i;
 
  radius = diameter >> 1;
  area = area_circle(radius);
  volume = area * height;
  return volume;
}

