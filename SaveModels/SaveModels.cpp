#include <QMap>
#include <QHash>
#include <QSet>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QVector>

#include <QMultiMap>
#include <QMultiHash>

#include <QString>

#include <QtDebug>
#include "velocitymodel.h"

void mapStringIntDemo()
{
    QMap<QString, int> map;

    map["foo"] = 42;
    map["bar"] = 13;
    map["baz"] = 9;

    QString key;
    foreach( key, map.keys() )
    qDebug() << key << " = " << map[key];

    QMap<QString, int>::ConstIterator ii;
    for ( ii = map.constBegin(); ii != map.constEnd(); ++ii )
        qDebug() << ii.key() << " = " << ii.value();

    int sum; 
    sum = map["foo"] + map.value("ingenting");

    foreach( key, map.keys() )
    qDebug() << key << " = " << map[key];

    sum = map["foo"] + map["ingenting"];

    foreach( key, map.keys() )
    qDebug() << key << " = " << map[key];
}

void hashStringIntDemo()
{
    QHash<QString, int> hash;

    hash["foo"] = 42;
    hash["bar"] = 13;
    hash["baz"] = 9;

    QString key;
    foreach( key, hash.keys() )
    qDebug() << key << " = " << hash[key];

    int sum; 
    sum = hash["foo"] + hash.value("ingenting");

    foreach( key, hash.keys() )
    qDebug() << key << " = " << hash[key];

    sum = hash["foo"] + hash["ingenting"];

    foreach( key, hash.keys() )
    qDebug() << key << " = " << hash[key];
}

class Person {
public:
    Person( QString name, QString number );

    QString name() const;
    QString number() const;

private:
    QString m_name, m_number;
};

Person::Person( QString name, QString number ) : m_name( name ), m_number( number ) {}
QString Person::name() const { return m_name;}
QString Person::number() const { return m_number;}

bool operator==( const Person &a, const Person &b )
{
    return(a.name() == b.name()) && (a.number() == b.number());
}

uint qHash( const Person &key )
{
    return qHash( key.name() ) ^ qHash( key.number() );
}

void hashPersons()
{
    QHash<Person, int> hash;

    hash[ Person( "Anders", "8447070" ) ] = 10;
    hash[ Person( "Micke", "7728433" ) ] = 20;

    qDebug() << hash.value( Person( "Anders", "8447070" ) ); // 10
    qDebug() << hash.value( Person( "Anders", "8447071" ) ); // 0
    qDebug() << hash.value( Person( "Micke", "7728433" ) ); // 20
    qDebug() << hash.value( Person( "Michael", "7728433" ) ); // 0
}

void setStringDemo()
{
    QSet<QString> set;

    set << "Ada" << "C++" << "Ruby";

    for ( QSet<QString>::ConstIterator ii = set.begin(); ii != set.end(); ++ii )
        qDebug() << *ii;

    if ( set.contains( "FORTRAN" ) )
        qDebug() << "FORTRAN is in the set.";
    else
        qDebug() << "FORTRAN is out.";
}

void multimapStringIntDemo()
{
    QMultiMap<QString, int> multi;

    multi.insert( "foo", 10 );
    multi.insert( "foo", 20 );
    multi.insert( "bar", 30 );

    QSet<QString> keys;
    foreach( QString key, multi.keys() )
    keys << key;

    foreach( QString key, keys )
    foreach( int value, multi.values(key) )
    qDebug() << key << ": " << value;

    QMultiMap<QString, int>::ConstIterator ii = multi.find( "foo" );
    while ( ii != multi.end() && ii.key() == "foo" ) {
        qDebug() << ii.value();
        ++ii;
    }
}

void multihashStringIntDemo()
{
    QMultiHash<QString, int> multi;

    multi.insert( "foo", 10 );
    multi.insert( "foo", 20 );
    multi.insert( "bar", 30 );

    QSet<QString> keys;
    foreach( QString key, multi.keys() )
    keys << key;

    foreach( QString key, keys )
    foreach( int value, multi.values(key) )
    qDebug() << key << ": " << value;

    QMultiHash<QString, int>::ConstIterator ii = multi.find( "foo" );
    while ( ii != multi.end() && ii.key() == "foo" ) {
        qDebug() << ii.value();
        ++ii;
    }
}

int main( void )
{
    mapStringIntDemo();
    hashStringIntDemo();
    hashPersons();
    setStringDemo();
    multimapStringIntDemo();
    multihashStringIntDemo();

    QMap<QString, VelocityModel> vmMap;

    VelocityModel vm1( "Modelo 1", QDir::currentPath() + "/model.out"  );
    vmMap.insert( "Modelo 1", vm1 );

    VelocityModel vm2( "Modelo 2", QDir::currentPath() + "/model2.out"  );
    vmMap.insert( "Modelo 2", vm2 );

    QVector<VelocityModel> myVector;
    QMap<QString, VelocityModel>::ConstIterator ii;
    for ( ii = vmMap.constBegin(); ii != vmMap.constEnd(); ++ii ) {
        qDebug() << ii.key() << " = " << ii.value().modelFile();
        ii.value();
        myVector.push_back(ii.value());
    }

    QFile file("mystuff.vsf");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//      QMessageBox::warning(this, tr("Simulation"),
//                           tr("Cannot write file %1:\n%2.")
//                           .arg(file.fileName())
//                           .arg(file.errorString()));
//      return false;
    }


    QTextStream out(&file);

    out << "" << endl;
    out << qPrintable(QString("%1").arg(myVector.size())) << endl;

    for ( int i = 0; i < myVector.size(); ++i ) {
        out << myVector[i].modelName() << " = " 
            << myVector[i].modelFile() << endl;
    }

//  for ( ii = vmMap.constBegin(); ii != vmMap.constEnd(); ++ii )
//      out << ii.key() << " = " << ii.value().modelFile() << endl;


    QFile file2("mystuff.vsf");
    if (!file2.open(QIODevice::ReadOnly | QIODevice::Text)) {
//      QMessageBox::warning(this, tr("Simulation"),
//                           tr("Cannot read file %1:\n%2.")
//                           .arg(file.fileName())
//                           .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file2);

    in.readLine().simplified();
    int num  = in.readLine().simplified().toInt();
    qDebug() << num;

    QString word;
    for (int i = 0; i < num ; ++i ) {
        QString item  = in.readLine().simplified();
        QStringList parts = item.split(" = ");
        qDebug() << parts[0];
        qDebug() << parts[1];
    }

    return 0;
}
