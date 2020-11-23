﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using NetVips;

namespace DotNetVIPS
{
    public partial class FrmMain : Form
    {
        public FrmMain()
        {
            InitializeComponent();
        }

        private void btnRead_Click(object sender, EventArgs e)
        {
            var im = NetVips.Image.NewFromFile("E:/Data/Datasets/LargeImages/TEST_large.png");
            // put im at position (100, 100) in a 3000 x 3000 pixel image, 
            // make the other pixels in the image by mirroring im up / down / 
            // left / right, see
            // https://libvips.github.io/libvips/API/current/libvips-conversion.html#vips-embed
            im = im.Embed(100, 100, 3000, 3000, extend: Enums.Extend.Mirror);

            // multiply the green (middle) band by 2, leave the other two alone
            im *= new[] { 1, 2, 1 };

            // make an image from an array constant, convolve with it
            var mask = NetVips.Image.NewFromArray(new[,] {
                {-1, -1, -1},
                {-1, 16, -1},
                {-1, -1, -1}
            }, 8);
            im = im.Conv(mask, precision: Enums.Precision.Integer);
            
            // finally, write the result back to a file on disk
            im.WriteToFile("E:\\Data\\Datasets\\LargeImages\\TEST_large_out.png");
        }
    }
}
