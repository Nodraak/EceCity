double ft_long(int window_max, double window_zoom, double window_offset, double val)
{
    double coord_min = 0 - window_offset;
    double coord_max = window_max/window_zoom - window_offset;
    int pxl_min = 0;
    int pxl_max = window_max;

    return pxl_min + (val-coord_min) * (pxl_max-pxl_min) / (coord_max-coord_min);
}

double ft_short(double window_zoom, double window_offset, double val)
{
    return (val+window_offset) * window_zoom;
}
