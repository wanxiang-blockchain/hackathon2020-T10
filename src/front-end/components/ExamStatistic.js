import { Statistic, Row, Col } from 'antd'

const { Countdown } = Statistic

const deadline = Date.now() + 1000 * 60 * 60 * 24 * 2 + 1000 * 30; // Moment is also OK

export default () => {

  const onFinish  = () =>  {}

  return (
    <>
    <Row gutter={16}>
      <Col span={12}>
        <Countdown valueStyle={{color: '#faad14'}} title="考试时间" value={deadline} onFinish={onFinish} />
      </Col>
      <Col span={12}>
        <Statistic valueStyle={{color: '#1890ff'}} title="考生人数" value={93} suffix="/ 108" />
      </Col>
    </Row>
    <Row gutter={16}>
      </Row>
    <style jsx>{`
      .count {
        color: red;
      }
    `}</style>
    </>
  )
}
