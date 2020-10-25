import { Statistic, Row, Col } from 'antd'

const { Countdown } = Statistic

const deadline = Date.now() + 1000 * 60 * 60 * 24 * 2 + 1000 * 30; // Moment is also OK

export default () => {

  const blockCount = 15324

  return (
    <>
    <Row gutter={16}>
      <Col span={12}>
        <Statistic valueStyle={{color: '#faad14'}} title="区块高度" value={blockCount} />
      </Col>
      <Col span={12}>
        <Statistic valueStyle={{color: '#1890ff'}} title="最新交易" value={'e379bb7bf'} />
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
