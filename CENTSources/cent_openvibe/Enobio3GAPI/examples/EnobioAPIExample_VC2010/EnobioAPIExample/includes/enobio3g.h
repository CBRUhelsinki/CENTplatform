#ifndef ENOBIO3G_H
#define ENOBIO3G_H

/*!
 * \mainpage
 *
 * The Enobio API consists on a set of classes that permit the access to the
 * Enobio device and the data that it provides.
 *
 * The main class to access the device is Enobio3G. This class provides methods
 * for opening the device, starting and stopping the data streaming and
 * configuring the device.
 *
 *
 * The access to both the data streaming and the device status is made through
 * a producer-consumer protocol. Enobio implements two producers, one for the
 * data streaming (Enobio3G::ENOBIO_DATA) and another one for its status
 * (Enobio3G::STATUS). By registering an implementation of the IDataConsumer
 * virtual class with the Enobio3G::registerConsumer method the link between
 * the poducer and the cosumer is made. Thus the Enobio class will call the
 * IDataConsumer::receiveData method of all registered IDataConsumer
 * implementations every time a new sample is available or the device status
 * changes.
 *
 * The actual data or status is passed as a parameter to the
 * IDataConsumer::receiveData implementation through the PData class. The
 * pointer returned by the PData::getData method shall be casted to the
 * ChannelData or StatusData class depending on whether the implementation of
 * the IDataConsumer is registered to the Enobio3G::ENOBIO_DATA producer or the
 * Enobio3G::STATUS one.
 *
 * The implementation of the IDataConsumer::receiveData method shall take into
 * consideration that it will be executed from a different thread than the
 * Enobio one. So the operations done within the implementation shall be
 * thread-safe.
 *
 * Another consideration that the IDataConsumer::receiveData implementation
 * shall take into account is that the data accessed through the PData::getData
 * method is not accessible out of the scope of that method. That data is
 * deleted inside the Enobio class when all the IDataConsumer implementations
 * are called.
 */

#include "Enobio3GAPI_global.h"
#include "IDataProcessor.h"

/*! \class Enobio3G Enobio3G.h
 *
 * \brief This class provides access to the Enobio device and its data
 * streaming.
 *
 * This class provides methods for opening the device, starting and stopping
 * the data streaming and configuring the device.
 *
 * It inheritates from the base class IDataProcessor so it provides producer
 * implementations to which IDataConsumer implementations can register to
 * access both streaming data and device status.
 */
class ENOBIO3GAPISHARED_EXPORT Enobio3G : public IDataProcessor {
public:

    /*!
     * Version id string
     */
    static const char * ENOBIO_API_VERSION;

    /*!
     * Constructor of the Enobio3G object
     */
    Enobio3G();

    /*!
     * Enobio destructor
     */
    virtual ~Enobio3G ();

    /*!
     * The ids of the available producers.
     */
    enum producerIDs
    {
        ENOBIO_DATA=1,
        STATUS=2
    };

    /*!
     * It performs the operations required to initialize the hardware
     */
    bool openDevice (unsigned char * macAddress);

    /*!
     * It performs the operations required to close the hardware device
     */
    void closeDevice ();

    /*!
     * Start sampling at the predefined sampling rate.
     */
    void startStreaming ();

    /*!
     * Hold sampling (setting sampling rate to 0)
     */
    void stopStreaming ();

    /*!
     * deprecated since the communication is keeped alive in the background.
     * If the device is not present it is reported through the status producer
     */
    bool ping ();

    /*!
     * It returns the DataConsumer object identified with an ID.
     *
     * \param id The identification of the DataConsumer to be obtained.
     *
     * \return Reference to the solicited DataConsumer.
     */
    virtual IDataConsumer * getDataConsumer (const int id);

    /*!
     * It returns the DataProducer object identified with an ID.
     *
     * \param id The identification of the DataProducer to be obtained.
     *
     * \return Reference to the solicited DataProducer.
     */
    virtual IDataProducer * getDataProducer (const int id);


    /*!
     * It adds a DataConsumer to the list of the consumers of the Producer
     * identified by the parameter id
     *
     * \param id Identification of the Producer to use
     * \param dataConsumer The DataConsumer to add to the Producer
     *
     * \return If the producer specified does not exist the function returns false
     *
     */
    virtual bool registerConsumer (const int id,
                                    IDataConsumer& dataConsumer);

    /*!
     * It removes a DataConsumer from the list of consumers of the
     * Producer identified by the parameter id
     *
     * \param id Identification of the Producer to use
     * \param dataConsumer The DataConsumer to remove from the Producer
     *
     * \return If the producer specified does not exist the function returns false
     *
     */
    virtual bool deregisterConsumer (const int id,
                                     const IDataConsumer& dataConsumer);

private:

    /*!
     * Instance of the implementation class.
     */
    void * _enobioPrivate;
};


#endif // ENOBIO3G_H
