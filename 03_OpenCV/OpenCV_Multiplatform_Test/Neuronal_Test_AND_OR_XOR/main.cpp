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

       int inputLayerSize = 2;
       int outputLayerSize = 1;
       int numSamples = 4;
       vector<int> layerSizes = { inputLayerSize, 2 ,outputLayerSize };
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
       samples.at<float>( Point( 0, 0 ) ) = -1.0f;
       samples.at<float>( Point( 0, 1 ) ) = -1.0f;
       samples.at<float>( Point( 0, 2 ) ) = 1.0f;
       samples.at<float>( Point( 0, 3 ) ) = 1.0f;
       samples.at<float>( Point( 1, 0 ) ) = -1.0f;
       samples.at<float>( Point( 1, 1 ) ) = 1.0f;
       samples.at<float>( Point( 1, 2 ) ) = -1.0f;
       samples.at<float>( Point( 1, 3 ) ) = 1.0f;
       Mat responses( Size( outputLayerSize, numSamples ), CV_32F );
       responses.at<float>( Point( 0, 0 ) ) = -1.0f;
       responses.at<float>( Point( 0, 1 ) ) = 1.0f;
       responses.at<float>( Point( 0, 2 ) ) = 1.0f;
       responses.at<float>( Point( 0, 3 ) ) = -1.0f;

       cout << "samples:\n" << samples << endl;
       cout << "\nresponses:\n" << responses << endl;

       if ( !nnPtr->train( samples, ml::ROW_SAMPLE, responses ) )
           return 1;

       cout << "\nweights[0]:\n" << nnPtr->getWeights( 0 ) << endl;
       cout << "\nweights[1]:\n" << nnPtr->getWeights( 1 ) << endl;
       cout << "\nweights[2]:\n" << nnPtr->getWeights( 2 ) << endl;
       cout << "\nweights[3]:\n" << nnPtr->getWeights( 3 ) << endl;

       Mat output;
       nnPtr->predict( samples, output );
       cout << "\noutput:\n" << output << endl;

      return 0;
}
