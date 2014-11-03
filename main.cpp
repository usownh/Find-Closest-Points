#include <QFile>
#include <QDebug>
#include <math.h>
class Point
{
public:
    Point(double _x=0,double _y=0) {x=_x;y=_y;}
    double DistanceToPoint(Point p);
    double GetX(){return x;}
    double GetY(){return y;}
private:
    double x,y;
};
enum sortType{
    sortByX,sortByY
};
double Point::DistanceToPoint(Point p)
{
    return sqrt((x-p.GetX())*(x-p.GetX())+(y-p.GetY())*(y-p.GetY()));
}
void Merge(QList<Point> &pointsList,int start,int mid,int end,sortType type)
{
    QList<Point> left;
    for(int i=start;i<=mid;i++) left.append(pointsList[i]);
    int leftIndex=0,rightIndex=mid+1;
    bool comp;
    for(int i=start;i<=end;i++)
    {
        if(rightIndex>end) comp=true;
        else if(leftIndex>=left.size()) comp=false;
        else
        {
            if(type==sortByX) comp=left[leftIndex].GetX()<pointsList[rightIndex].GetX();
            else comp=left[leftIndex].GetY()<pointsList[rightIndex].GetY();
        }
        if(comp)
        {
            pointsList.replace(i,left[leftIndex++]);
        }
        else pointsList.replace(i,pointsList[rightIndex++]);
    }
}
void Sort(QList<Point> &pointsList,int start,int end, sortType type)
{
    if(end>start)
    {
        int mid=(start+end)/2;
        Sort(pointsList,start,mid,type);
        Sort(pointsList,mid+1,end,type);
        Merge(pointsList,start,mid,end,type);
    }
}
void Sort(QList<Point> &pointsList,sortType type)
{
    Sort(pointsList,0,pointsList.size()-1,type);
}
void FindClosestPoints(QList<Point> &pointsList, double &distance,Point &result1, Point &result2)
{
    double dis;
    for(int start=0;start<pointsList.size();start++)
        for(int i=1;i<=11&&start+i<pointsList.size();i++)
        {
            dis=pointsList[start].DistanceToPoint(pointsList[start+i]);
            if(dis<distance)
            {
                distance=dis;
                result1=pointsList[start];
                result2=pointsList[start+i];
            }
        }
}
double FindClosestPoints(QList<Point> &pointsList,int start,int end,Point &result1,Point &result2)
{
    double distance;
    if(end-start==1)
    {
        result1=pointsList[start];
        result2=pointsList[end];
        distance=result1.DistanceToPoint(result2);
    }
    else
    {
        Point left1,left2;
        Point right1,right2;
        double distance1,distance2;
        int mid=(end+start)/2;
        distance1=FindClosestPoints(pointsList,start,mid,left1,left2);
        distance2=FindClosestPoints(pointsList,mid+1,end,right1,right2);
        if(distance1>=distance2)
        {
            result1=left1;
            result2=left2;
            distance=distance1;
        }
        else
        {
            result1=right1;
            result2=right2;
            distance=distance2;
        }
        QList<Point> tmpList;
        double x=pointsList[mid].GetX();
        for(int i=start;i<=end;i++)
        {
            if(pointsList[i].GetX()<=x-distance) continue;
            else
            {
                start=i;
                break;
            }
        }
        for(int i=end;i>=start;i--)
        {
            if(pointsList[i].GetX()>=x+distance) continue;
            else
            {
                end=i;
                break;
            }
        }
        tmpList=pointsList.mid(start,end-start+1);
        Sort(tmpList,sortByY);
        FindClosestPoints(tmpList,distance,result1,result2);
    }
    return distance;
}
int main(int argc, char *argv[])
{
    QFile file;
    QList<Point> pointsList;
    file.setFileName("points.txt");
    file.open(QIODevice::ReadOnly);
    QString line;
    while(!file.atEnd())
    {
        line=file.readLine();
        pointsList.append(Point(line.mid(0,line.indexOf(",")).toDouble(),line.mid(line.indexOf(",")+1).toDouble()));
    }
    file.close();
    Point result1,result2;
    double distance;
    Sort(pointsList,sortByX);
    distance=FindClosestPoints(pointsList,0,pointsList.size()-1,result1,result2);
    qDebug()<<QString("the closest points pair are (%1,%2) and (%3,%4), the distance is %5")
              .arg(result1.GetX()).arg(result1.GetY())
              .arg(result2.GetX()).arg(result2.GetY())
              .arg(distance);
    return 0;
}
