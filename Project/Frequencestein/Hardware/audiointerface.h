#ifndef AUDIOINTERFACE_H
#define AUDIOINTERFACE_H
#include <QAudioInput>
#include <QScopedPointer>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QIODevice>

class AudioInfo : public QIODevice
{
    Q_OBJECT

public:
    AudioInfo(const QAudioFormat &format);

    void start();
    void stop();

    qreal level() const { return m_level; }

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

    quint32* rawData;

private:
    const QAudioFormat m_format;
    quint32 m_maxAmplitude = 0;
    qreal m_level = 0.0; // 0.0 <= m_level <= 1.0

signals:
    void update();
};


class AudioInterface
{
private:
    QScopedPointer<QAudioInput> audioInput;
public:
    QScopedPointer<AudioInfo> audioInfo;
    qreal level = 0.0;
    int sampleSize;
    AudioInterface();
    ~AudioInterface();
    void start();
    void stop();
    qreal getValue();
    quint32* getSample();
};

#endif // AUDIOINTERFACE_H
