import { Statistic, Row, Col } from 'antd'

const { Countdown } = Statistic

const deadline = Date.now() + 1000 * 60 * 60 * 24 * 2 + 1000 * 30; // Moment is also OK

export default () => {

  const totalPen = '64k';
  const totalCustomer = 1024

  return (
    <>
    <Row gutter={16}>
      <Col span={12}>
        <Statistic valueStyle={{color: ''}} title="总笔数" value={totalPen} /> 
      </Col>
      <Col span={12}>
        <Statistic  title="使用人次" value={totalCustomer}  />
      </Col>
    </Row>
    <Row gutter={16}>
    </Row>
    </>
  )
}
