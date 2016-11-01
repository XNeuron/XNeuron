#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main(int, char**)
{
       //0 = Xor; 0.1 = Or; 0.2 = And; 0.3 = Add
       int inputLayerSize = 4;
       int outputLayerSize = 1;
       int numSamples = 16;
       vector<int> layerSizes = { inputLayerSize, 3 ,outputLayerSize };
       Ptr<ml::ANN_MLP> nnPtr = ml::ANN_MLP::create();
       nnPtr->setLayerSizes( layerSizes );
       nnPtr->setActivationFunction( cv::ml::ANN_MLP::SIGMOID_SYM );
       nnPtr->setTrainMethod(ml::ANN_MLP::BACKPROP,0.01,0.01);
       TermCriteria tc;
       tc.epsilon=0;
       tc.maxCount=1000000;
       tc.type = TermCriteria::MAX_ITER + TermCriteria::EPS;
       nnPtr->setTermCriteria(tc);

       Mat samples( Size( inputLayerSize, numSamples ), CV_32F );
      //Xor
       samples.at<float>( Point( 0, 0 ) ) = -1.0f;
       samples.at<float>( Point( 0, 1 ) ) = -1.0f;
       samples.at<float>( Point( 0, 2 ) ) = 1.0f;
       samples.at<float>( Point( 0, 3 ) ) = 1.0f;

       samples.at<float>( Point( 1, 0 ) ) = -1.0f;
       samples.at<float>( Point( 1, 1 ) ) = 1.0f;
       samples.at<float>( Point( 1, 2 ) ) = -1.0f;
       samples.at<float>( Point( 1, 3 ) ) = 1.0f;

       samples.at<float>( Point( 2, 0 ) ) = 0.0f;
       samples.at<float>( Point( 2, 1 ) ) = 0.0f;
       samples.at<float>( Point( 2, 2 ) ) = 0.0f;
       samples.at<float>( Point( 2, 3 ) ) = 0.0f;

       //Or
        samples.at<float>( Point( 0, 4 ) ) = -1.0f;
        samples.at<float>( Point( 0, 5 ) ) = -1.0f;
        samples.at<float>( Point( 0, 6 ) ) = 1.0f;
        samples.at<float>( Point( 0, 7 ) ) = 1.0f;

        samples.at<float>( Point( 1, 4 ) ) = -1.0f;
        samples.at<float>( Point( 1, 5 ) ) = 1.0f;
        samples.at<float>( Point( 1, 6 ) ) = -1.0f;
        samples.at<float>( Point( 1, 7 ) ) = 1.0f;

        samples.at<float>( Point( 2, 4 ) ) = 0.1f;
        samples.at<float>( Point( 2, 5 ) ) = 0.1f;
        samples.at<float>( Point( 2, 6 ) ) = 0.1f;
        samples.at<float>( Point( 2, 7 ) ) = 0.1f;

        //And
        samples.at<float>( Point( 0, 8 ) ) = -1.0f;
        samples.at<float>( Point( 0, 9 ) ) = -1.0f;
        samples.at<float>( Point( 0, 10 ) ) = 1.0f;
        samples.at<float>( Point( 0, 11 ) ) = 1.0f;

        samples.at<float>( Point( 1, 8 ) ) = -1.0f;
        samples.at<float>( Point( 1, 9 ) ) = 1.0f;
        samples.at<float>( Point( 1, 10 ) ) = -1.0f;
        samples.at<float>( Point( 1, 11 ) ) = 1.0f;

        samples.at<float>( Point( 2, 8 ) ) = 0.2f;
        samples.at<float>( Point( 2, 9 ) ) = 0.2f;
        samples.at<float>( Point( 2, 10 ) ) = 0.2f;
        samples.at<float>( Point( 2, 11 ) ) = 0.2f;

        //Add
        samples.at<float>( Point( 0, 12 ) ) = 0.100f;
        samples.at<float>( Point( 0, 13 ) ) = 0.045f;
        samples.at<float>( Point( 0, 14 ) ) = 0.03f;
        samples.at<float>( Point( 0, 15 ) ) = 0.012f;

        samples.at<float>( Point( 1, 12 ) ) = 0.100f;
        samples.at<float>( Point( 1, 13 ) ) = 0.015f;
        samples.at<float>( Point( 1, 14 ) ) = 0.050f;
        samples.at<float>( Point( 1, 15 ) ) = 0.0f;

        samples.at<float>( Point( 2, 12 ) ) = 0.3f;
        samples.at<float>( Point( 2, 13 ) ) = 0.3f;
        samples.at<float>( Point( 2, 14 ) ) = 0.3f;
        samples.at<float>( Point( 2, 15 ) ) = 0.3f;


        Mat responses( Size( outputLayerSize, numSamples ), CV_32F );

        //Xor
       responses.at<float>( Point( 0, 0 ) ) = -1.0f;
       responses.at<float>( Point( 0, 1 ) ) = 1.0f;
       responses.at<float>( Point( 0, 2 ) ) = 1.0f;
       responses.at<float>( Point( 0, 3 ) ) = -1.0f;

       //Or
       responses.at<float>( Point( 0, 4 ) ) = -1.0f;
       responses.at<float>( Point( 0, 5 ) ) = 1.0f;
       responses.at<float>( Point( 0, 6 ) ) = 1.0f;
       responses.at<float>( Point( 0, 7 ) ) = 1.0f;

       //And
       responses.at<float>( Point( 0, 8 ) ) = -1.0f;
       responses.at<float>( Point( 0, 9 ) ) = -1.0f;
       responses.at<float>( Point( 0, 10 ) ) = -1.0f;
       responses.at<float>( Point( 0, 11 ) ) = 1.0f;

       //Add
       responses.at<float>( Point( 0, 12 ) ) = 0.200f;
       responses.at<float>( Point( 0, 13 ) ) = 0.060f;
       responses.at<float>( Point( 0, 14 ) ) = 0.080f;
       responses.at<float>( Point( 0, 15 ) ) = 0.012f;

       cout << "samples:\n" << samples << endl;
       cout << "\nresponses:\n" << responses << endl;

       if ( !nnPtr->train( samples, ml::ROW_SAMPLE, responses ) )
           return 1;

       cout << "\nweights[0]:\n" << nnPtr->getWeights( 0 ) << endl;
       cout << "\nweights[1]:\n" << nnPtr->getWeights( 1 ) << endl;
       cout << "\nweights[2]:\n" << nnPtr->getWeights( 2 ) << endl;
       cout << "\nweights[3]:\n" << nnPtr->getWeights( 3 ) << endl;

       //Add test values:

       //Add
       samples.at<float>( Point( 0, 12 ) ) = 0.01f;
       samples.at<float>( Point( 0, 13 ) ) = 0.001f;
       samples.at<float>( Point( 0, 14 ) ) = 0.025f;
       samples.at<float>( Point( 0, 15 ) ) = -0.010f;

       samples.at<float>( Point( 1, 12 ) ) = 0.01f;
       samples.at<float>( Point( 1, 13 ) ) = 0.02f;
       samples.at<float>( Point( 1, 14 ) ) = 0.025f;
       samples.at<float>( Point( 1, 15 ) ) = 0.005f;

       samples.at<float>( Point( 2, 12 ) ) = 0.3f;
       samples.at<float>( Point( 2, 13 ) ) = 0.3f;
       samples.at<float>( Point( 2, 14 ) ) = 0.3f;
       samples.at<float>( Point( 2, 15 ) ) = 0.3f;

       responses.at<float>( Point( 0, 12 ) ) = 0.02f;
       responses.at<float>( Point( 0, 13 ) ) = 0.021f;
       responses.at<float>( Point( 0, 14 ) ) = 0.050f;
       responses.at<float>( Point( 0, 15 ) ) = -0.005f;

       Mat output;
       nnPtr->predict( samples, output );
       cout << "\noutput:\n" << output << endl;

      return 0;
}
