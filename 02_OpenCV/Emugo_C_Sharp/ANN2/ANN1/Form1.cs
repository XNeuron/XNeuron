using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Drawing;
using Emgu.CV.Structure;
using Emgu.CV.ML;
using Emgu.CV.ML.Structure;
using Emgu.CV;
using System.IO;

namespace ANN1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {

            Matrix<int> layerSize = new Matrix<int>(new int[] { 2, 2, 1 });
            ANN_MLP nnPtr = new ANN_MLP();
            nnPtr.SetLayerSizes(layerSize);
            nnPtr.SetActivationFunction(ANN_MLP.AnnMlpActivationFunction.SigmoidSym);
            nnPtr.SetTrainMethod(ANN_MLP.AnnMlpTrainMethod.Backprop, 0.01, 0.01);

            if (!nnPtr->Train(samples, ROW_SAMPLE, responses))
                return 1;
        }
    }
}
